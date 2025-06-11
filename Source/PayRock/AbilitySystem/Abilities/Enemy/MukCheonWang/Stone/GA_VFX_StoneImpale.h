// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "StoneSpikeActor.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "PayRock/AbilitySystem/Abilities/Enemy/MukCheonWang/GA_Boss_Magic_Base.h"
#include "GA_VFX_StoneImpale.generated.h"

UCLASS()
class PAYROCK_API UGA_VFX_StoneImpale : public UGA_Boss_Magic_Base
{
	GENERATED_BODY()

public:
	UGA_VFX_StoneImpale();

protected:
	virtual void OnAuraEffectComplete() override;
	void SpawnSingleSpike();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Stone")
	TSubclassOf<AStoneSpikeActor> StoneSpikeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Stone")
	float SpawnDistance = 800.f;

	TArray<TWeakObjectPtr<AActor>> SpikeTargets;
	int32 CurrentTargetIndex = 0;

	FTimerHandle SpikeSpawnTimerHandle;
};