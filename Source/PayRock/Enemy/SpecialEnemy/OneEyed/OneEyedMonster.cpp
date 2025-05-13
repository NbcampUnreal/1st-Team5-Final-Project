// PayRockGames

#include "OneEyedMonster.h"
#include "OneEyedMonsterController.h"


AOneEyedMonster::AOneEyedMonster()
{
	AIControllerClass = AOneEyedMonsterController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}


void AOneEyedMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

