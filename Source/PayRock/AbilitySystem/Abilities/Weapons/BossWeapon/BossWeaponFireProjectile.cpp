// PayRockGames

#include "BossWeaponFireProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABossWeaponFireProjectile::ABossWeaponFireProjectile(): InterpSpeed(2.5f)
{
}

void ABossWeaponFireProjectile::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;
}

void ABossWeaponFireProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!TargetActor.IsValid())
	{
		Destroy();
	}
    
	FVector CurrentVelocity = ProjectileMovement->Velocity;
	FVector DirectionToTarget = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FVector NewVelocity = FMath::VInterpTo(CurrentVelocity, DirectionToTarget * ProjectileSpeed, DeltaTime, InterpSpeed);
    
	ProjectileMovement->Velocity = NewVelocity;
}

