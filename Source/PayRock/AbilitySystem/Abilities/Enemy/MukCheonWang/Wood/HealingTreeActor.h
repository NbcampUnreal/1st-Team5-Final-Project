// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Enemy/EnemyCharacter.h" // AEnemyCharacter 기반
#include "HealingTreeActor.generated.h"

class AMukCheonWangCharacter;

UCLASS()
class PAYROCK_API AHealingTreeActor : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	AHealingTreeActor();

	void SetBoss(AMukCheonWangCharacter* InBoss);

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<AMukCheonWangCharacter> Boss;

	UPROPERTY(EditDefaultsOnly, Category = "Healing")
	float HealPerSecond = 1.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Healing")
	float TickInterval = 1.0f;

	FTimerHandle HealTimerHandle;

	UFUNCTION()
	void HealBoss();
};