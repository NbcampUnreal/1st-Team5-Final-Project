// PayRockGames

#include "SkeletonMonster.h"
#include "SkeletonMonsterController.h"

ASkeletonMonster::ASkeletonMonster()
{
	AIControllerClass = ASkeletonMonsterController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ASkeletonMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

