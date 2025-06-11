// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "PayRock/AbilitySystem/Abilities/Enemy/MukCheonWang/GA_Boss_Magic_Base.h"
#include "GA_VFX_LightningStrike.generated.h"


class ALightningStrikeActor;

UCLASS()
class PAYROCK_API UGA_VFX_LightningStrike : public UGA_Boss_Magic_Base
{
	GENERATED_BODY()

public:
	UGA_VFX_LightningStrike();

protected:
	virtual void OnAuraEffectComplete() override;

	void SpawnLightningForTarget();
	void SpawnNextLightning();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Lightning")
	TSubclassOf<ALightningStrikeActor> LightningClass;

	UPROPERTY(EditDefaultsOnly, Category = "Lightning")
	float PredictDelay = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Lightning")
	float LightningSpawnDelay = 0.4f;

	TArray<TWeakObjectPtr<AActor>> LightningTargets;
	int32 CurrentTargetIndex = 0;
	int32 NumLightningPerTarget = 0;
	int32 CurrentLightningCount = 0;

	FVector CurrentLightningCenter;
	FTimerHandle LightningChainTimer;
};