// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "HealingTreeActor.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "PayRock/AbilitySystem/Abilities/Enemy/MukCheonWang/GA_Boss_Magic_Base.h"
#include "GA_SpawnHealingTrees.generated.h"

UCLASS()
class PAYROCK_API UGA_SpawnHealingTrees : public UGA_Boss_Magic_Base
{
	GENERATED_BODY()

public:
	UGA_SpawnHealingTrees();

protected:
	virtual void OnAuraEffectComplete() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Trees")
	TSubclassOf<AHealingTreeActor> TreeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Trees")
	int32 MinSpawnCount = 2;

	UPROPERTY(EditDefaultsOnly, Category = "Trees")
	int32 MaxSpawnCount = 4;

	UPROPERTY(EditDefaultsOnly, Category = "Trees")
	float SpawnAreaWidth = 600.f;

	UPROPERTY(EditDefaultsOnly, Category = "Trees")
	float MinSpawnDistance = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "Trees")
	float MaxSpawnDistance = 700.f;
};