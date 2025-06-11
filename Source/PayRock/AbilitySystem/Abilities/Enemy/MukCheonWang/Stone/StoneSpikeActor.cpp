#include "StoneSpikeActor.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "PayRock/Character/PRCharacter.h"

AStoneSpikeActor::AStoneSpikeActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	CapsuleCollision->InitCapsuleSize(60.f, 100.f);
	CapsuleCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	SetRootComponent(CapsuleCollision);

	CollisionComponent = CapsuleCollision;

	VFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFX"));
	VFX->SetupAttachment(RootComponent);
	VFX->SetAutoActivate(false);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->InitialSpeed = BaseSpeed;
	MovementComp->MaxSpeed = BaseSpeed;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->ProjectileGravityScale = 0.f;
}

void AStoneSpikeActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		Multicast_PlaySpikeVFX();
		
		GetWorldTimerManager().SetTimer(
		VFXRepeatTimerHandle,
		this,
		&AStoneSpikeActor::PlayRepeatedSpikeVFX,
		0.25f,
		true
	);
	}

	SetLifeSpan(15.f);
}

void AStoneSpikeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStoneSpikeActor::OnEffectOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
									   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
									   bool bFromSweep, const FHitResult& SweepResult)
{
	ApplyDamageEffect(OtherActor);

	if (APRCharacter* HitCharacter = Cast<APRCharacter>(OtherActor))
	{
		const FVector LaunchVelocity = FVector(0.f, 0.f, 500.f);
		HitCharacter->LaunchCharacter(LaunchVelocity, true, true);
	}
	
	SetActorEnableCollision(false);

	if (VFX)
	{
		VFX->Deactivate(); 
		VFX->ActivateSystem();
	}
	
	GetWorldTimerManager().SetTimer(
		DestroyDelayHandle,
		this,
		&AStoneSpikeActor::DestroySpike,
		1.0f, 
		false
	);
}


void AStoneSpikeActor::SetTarget(AActor* InTarget)
{
	if (!HasAuthority()) return;

	Multicast_SetTarget(InTarget);
}

void AStoneSpikeActor::Multicast_SetTarget_Implementation(AActor* InTarget)
{
	TargetActor = InTarget;

	if (MovementComp && TargetActor && TargetActor->GetRootComponent())
	{
		MovementComp->bIsHomingProjectile = true;
		MovementComp->HomingAccelerationMagnitude = 1000.f;
		MovementComp->HomingTargetComponent = TargetActor->GetRootComponent();
	}
}

void AStoneSpikeActor::Multicast_PlaySpikeVFX_Implementation()
{
	if (VFX)
	{
		VFX->Activate(true);
	}
}

void AStoneSpikeActor::PlayRepeatedSpikeVFX()
{
	if (VFX)
	{
		if (VFX)
		{
			VFX->ResetSystem();
			VFX->Activate(true);
		}
	}
}

void AStoneSpikeActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(VFXRepeatTimerHandle);
	Super::EndPlay(EndPlayReason);
}

void AStoneSpikeActor::DestroySpike()
{
	GetWorldTimerManager().ClearTimer(DestroyDelayHandle);
	Destroy();
}
