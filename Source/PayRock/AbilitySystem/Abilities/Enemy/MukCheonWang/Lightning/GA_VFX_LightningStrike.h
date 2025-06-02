// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "GA_VFX_LightningStrike.generated.h"


class ALightningStrikeActor;

UCLASS()
class PAYROCK_API UGA_VFX_LightningStrike : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_VFX_LightningStrike();

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lightning")
	TSubclassOf<ALightningStrikeActor> LightningClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lightning")
	float PredictDelay = 0.8f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timing")
	float AuraDelayTime = 0.8f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timing")
	float LightningSpawnDelay = 0.3f;

	UPROPERTY()
	TObjectPtr<AActor> AvatarActor;


	FTimerHandle AuraDelayTimerHandle;
	FTimerHandle LightningChainTimer;


	FGameplayAbilitySpecHandle CurrentSpecHandle;
	FGameplayAbilityActorInfo* CurrentActorInfo = nullptr;
	FGameplayAbilityActivationInfo CurrentActivationInfo;


	TArray<AActor*> LightningTargets;
	int32 CurrentTargetIndex = 0;


	int32 NumLightningPerTarget = 0;
	int32 CurrentLightningCount = 0;
	FVector CurrentLightningCenter;


	UFUNCTION()
	void SpawnLightningAfterAura();

	void SpawnLightningForTarget();

	UFUNCTION()
	void SpawnNextLightning();
};