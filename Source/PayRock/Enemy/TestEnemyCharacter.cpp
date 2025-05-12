// PayRockGames


#include "TestEnemyCharacter.h"

#include "EnemyController.h"

ATestEnemyCharacter::ATestEnemyCharacter()
{
	AIControllerClass = AEnemyController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
