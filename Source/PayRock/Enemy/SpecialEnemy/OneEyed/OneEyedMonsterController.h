// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Enemy/EnemyController.h"
#include "OneEyedMonsterController.generated.h"

UCLASS()
class PAYROCK_API AOneEyedMonsterController : public AEnemyController
{
	GENERATED_BODY()

public:
	AOneEyedMonsterController();

protected:
	virtual void BeginPlay() override;
	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus) override;

	UPROPERTY(EditAnywhere, Category = "Light Detection")
	float LightDetectionThreshold = 400.0f;

private:
	TWeakObjectPtr<AActor> CachedTarget;
};