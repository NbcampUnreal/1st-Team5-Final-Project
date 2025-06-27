// PayRockGames

#include "BossWeaponFireProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PayRock/Character/PRCharacter.h"

ABossWeaponFireProjectile::ABossWeaponFireProjectile(): InterpSpeed(2.5f), TargetActor(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABossWeaponFireProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsValid(TargetActor))
	{
		Destroy();
		return;
	}
	if (APRCharacter* Character = Cast<APRCharacter>(TargetActor))
	{
		if (Character->GetbIsDead())
		{
			Destroy();
			return;	
		}
	}
    
	FVector CurrentVelocity = ProjectileMovement->Velocity;
	FVector DirectionToTarget = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FVector NewVelocity = FMath::VInterpTo(CurrentVelocity, DirectionToTarget * ProjectileSpeed, DeltaTime, InterpSpeed);
    
	ProjectileMovement->Velocity = NewVelocity;
}

