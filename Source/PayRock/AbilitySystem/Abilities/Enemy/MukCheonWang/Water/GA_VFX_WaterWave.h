// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "WaterWave.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "GA_VFX_WaterWave.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UGA_VFX_WaterWave : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_VFX_WaterWave();

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wave")
	TSubclassOf<AWaterWave> WaveClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wave")
	FVector WaveScale = FVector(1.0f, 1.0f, 1.0f);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wave")
	bool bWaveAlongX = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timing")
	float AuraDelayTime = 0.8f;
	
	FTimerHandle AuraDelayTimerHandle;
	FTimerHandle WaveChainTimer;
	
	UFUNCTION()
	void SpawnWaterWaveAfterAura();
	void SpawnNextWave();

	FGameplayAbilitySpecHandle CurrentSpecHandle;
	FGameplayAbilityActorInfo* CurrentActorInfo = nullptr;
	FGameplayAbilityActivationInfo CurrentActivationInfo;
	
	int32 NumWavesToSpawn = 0;
	int32 SpawnedWaveCount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timing")
	float WaveSpawnDelay = 1.8f;
	
	UPROPERTY()
	TObjectPtr<AActor> AvatarActor;
};