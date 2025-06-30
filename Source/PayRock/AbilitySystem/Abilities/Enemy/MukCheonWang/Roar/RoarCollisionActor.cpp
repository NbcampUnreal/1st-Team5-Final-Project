#include "RoarCollisionActor.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


ARoarCollisionActor::ARoarCollisionActor()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RoarSphere"));
	CollisionSphere->InitSphereRadius(10.f); 
	CollisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetGenerateOverlapEvents(true);
	RootComponent = CollisionSphere;

	
	RoarEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RoarEffect"));
	RoarEffect->SetupAttachment(RootComponent);
	RoarEffect->SetAutoActivate(false);
	
	CollisionSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARoarCollisionActor::OnOverlapBegin);
}

void ARoarCollisionActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		Multicast_PlayRoarEffect();
	}
}


void ARoarCollisionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;
	const float Alpha = FMath::Clamp(ElapsedTime / ExpansionDuration, 0.f, 1.f);
	const float CurrentRadius = FMath::Lerp(10.f, MaxRadius, Alpha);

	CollisionSphere->SetSphereRadius(CurrentRadius);
	
	if (Alpha >= 1.f)
	{
		Destroy();
	}
}

void ARoarCollisionActor::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (ACharacter* Player = Cast<ACharacter>(OtherActor))
	{
		const FVector Direction = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		const FVector Knockback = Direction * KnockbackStrength;
		
		if (UAnimInstance* AnimInstance = Player->GetMesh()->GetAnimInstance())
		{
			if (AnimInstance->IsAnyMontagePlaying())
			{
				AnimInstance->Montage_Stop(0.2f); 
			}
		}
		
		Player->LaunchCharacter(Knockback, true, true);
	}
}


void ARoarCollisionActor::Multicast_PlayRoarEffect_Implementation()
{
	if (RoarEffect)
	{
		RoarEffect->Activate(true);
	}
}