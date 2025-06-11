// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "WaterWave.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "PayRock/AbilitySystem/Abilities/Enemy/MukCheonWang/GA_Boss_Magic_Base.h"
#include "GA_VFX_WaterWave.generated.h"


UCLASS()
class PAYROCK_API UGA_VFX_WaterWave : public UGA_Boss_Magic_Base
{
	GENERATED_BODY()

public:
	UGA_VFX_WaterWave();

protected:
	virtual void OnAuraEffectComplete() override;
	void SpawnNextWave();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Wave")
	TSubclassOf<AWaterWave> WaveClass;

	UPROPERTY(EditDefaultsOnly, Category = "Wave")
	FVector WaveScale = FVector(1.f);

	UPROPERTY(EditDefaultsOnly, Category = "Wave")
	float WaveSpawnDelay = 0.6f;

	int32 NumWavesToSpawn = 1;
	int32 SpawnedWaveCount = 0;

	FTimerHandle WaveChainTimer;
};
