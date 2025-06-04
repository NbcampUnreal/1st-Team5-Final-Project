// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "LightSourceActor.h"
#include "OrbLightActor.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "GA_Light.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UGA_Light : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_Light();

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	
	UPROPERTY()
	TArray<TObjectPtr<ALightSourceActor>> AffectedLightActors;
	
	UPROPERTY(EditDefaultsOnly, Category = "LightSurvival")
	TSubclassOf<AOrbLightActor> OrbLightClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "LightSurvival")
	float LightDetectionRadius = 3000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "LightSurvival")
	float PatternDuration = 15.f;

	FTimerHandle EndPatternTimerHandle;

	UFUNCTION()
	void EndLightSurvivalPattern();

	void DisableNearbyLights(AActor* OriginActor);
	void RestoreLights();
	void SpawnLightOrbAtCenter(AActor* OriginActor);

	UPROPERTY()
	TObjectPtr<AOrbLightActor> SpawnedOrb;
};