// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "LightSourceActor.h"
#include "OrbLightActor.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "PayRock/AbilitySystem/Abilities/Enemy/MukCheonWang/GA_Boss_Magic_Base.h"
#include "GA_Light.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UGA_Light : public UGA_Boss_Magic_Base
{
	GENERATED_BODY()

public:
	UGA_Light();

protected:
	virtual void OnAuraEffectComplete() override;

private:
	
	void DisableNearbyLights(AActor* OriginActor);
	
	void SpawnLightOrbAtCenter(AActor* OriginActor);

	void EndLightSurvivalPattern();
	
	void RestoreLights();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Light")
	TSubclassOf<AOrbLightActor> OrbLightClass;

	UPROPERTY(EditDefaultsOnly, Category = "Light")
	float PatternDuration = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Light")
	float LightDetectionRadius = 3000.f;

private:
	UPROPERTY()
	TArray<ALightSourceActor*> AffectedLightActors;

	UPROPERTY()
	TWeakObjectPtr<AOrbLightActor> SpawnedOrb;

	FTimerHandle EndPatternTimerHandle;
};