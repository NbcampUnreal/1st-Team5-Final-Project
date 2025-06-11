#include "StoneSpikeActor.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

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
	Destroy();
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
