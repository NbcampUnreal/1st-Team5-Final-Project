// PayRockGames


#include "SamuraiStatueCharacter.h"

#include "EnemyController.h"
#include "GameFramework/CharacterMovementComponent.h"

ASamuraiStatueCharacter::ASamuraiStatueCharacter()
{
	
	AIControllerClass = AEnemyController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
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
