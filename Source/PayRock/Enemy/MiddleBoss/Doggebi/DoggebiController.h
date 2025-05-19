// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Enemy/EnemyController.h"
#include "DoggebiController.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API ADoggebiController : public AEnemyController
{
	GENERATED_BODY()

	public:
	ADoggebiController();
	
	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus) override;
};
