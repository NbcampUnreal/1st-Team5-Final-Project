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
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	
	void CheckLightBasedDetection();

	UPROPERTY(EditAnywhere, Category = "Light Detection")
	float LightDetectionThreshold = 400.0f;

private:
	AActor* CachedTarget = nullptr;
};