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
	
	UPROPERTY(EditDefaultsOnly, Category = "Wave")
	TSubclassOf<AWaterWave> WaveClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Wave")
	bool bWaveAlongX = true;
	
	UPROPERTY(EditDefaultsOnly, Category = "Wave")
	FVector WaveScale = FVector(1.f, 10.f, 1.f);
};