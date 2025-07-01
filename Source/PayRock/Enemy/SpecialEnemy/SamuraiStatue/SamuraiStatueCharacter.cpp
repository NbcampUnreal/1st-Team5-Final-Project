// PayRockGames


#include "SamuraiStatueCharacter.h"

#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "PayRock/Enemy/EnemyController.h"
#include "PayRock/AbilitySystem/Abilities/Enemy/GA_EnemyNormalAttack.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PayRock/Character/PRCharacter.h"

ASamuraiStatueCharacter::ASamuraiStatueCharacter()
{
	CharacterType = ECharacterType::SamuraiStatue;

	AIControllerClass = AEnemyController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightHandCollision"));
	WeaponCollision->SetupAttachment(Weapon);

	if (WeaponCollision)
	{
		WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &ASamuraiStatueCharacter::OnWeaponOverlap);
	}
}

void ASamuraiStatueCharacter::PauseAnimation()
{
	GetMesh()->bPauseAnims = true;
	GetCharacterMovement()->DisableMovement();
}

void ASamuraiStatueCharacter::ResumeAnimation()
{
	GetMesh()->bPauseAnims = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void ASamuraiStatueCharacter::SetDie(bool isdie)
{
	bIsDead = isdie;
}

bool ASamuraiStatueCharacter::GetDie()
{
	return bIsDead;
}


void ASamuraiStatueCharacter::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this || !CurrentSamuraiAttackGA) return;

	//if (Cast<AEnemyCharacter>(OtherActor)) return;

	if (HasAuthority())
	{
		APRCharacter* PlayerChar = Cast<APRCharacter>(OtherActor);
		if (PlayerChar && !bHit)
		{
			bHit = true;
			CurrentSamuraiAttackGA->CauseDamage(OtherActor, SweepResult);
		}
	}
}

void ASamuraiStatueCharacter::ToggleColiision(bool isActive)
{
	if (WeaponCollision)
	{
		WeaponCollision->SetCollisionEnabled(
			isActive ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision
		);
	}
}

void ASamuraiStatueCharacter::Die(FVector HitDirection)
{
	Super::Die(HitDirection);
	bIsDead = true;
}
