// PayRockGames


#include "SamuraiStatueCharacter.h"

#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "PayRock/Enemy/EnemyController.h"
#include "GameFramework/CharacterMovementComponent.h"

ASamuraiStatueCharacter::ASamuraiStatueCharacter()
{
	
	AIControllerClass = AEnemyController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightHandCollision"));
	WeaponCollision->SetupAttachment(Weapon);
	
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
