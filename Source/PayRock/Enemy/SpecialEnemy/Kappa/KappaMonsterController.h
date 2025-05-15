// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Enemy/EnemyController.h"
#include "KappaMonsterController.generated.h"

UCLASS()
class PAYROCK_API AKappaMonsterController : public AEnemyController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
};