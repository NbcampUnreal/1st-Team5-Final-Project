// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseGameplayAbility.h"
#include "BaseAreaEffectAbility.generated.h"

class AApplyEffectZone;

UCLASS(Blueprintable)
class PAYROCK_API UBaseAreaEffectAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void SpawnEffectArea();
	UFUNCTION()
	void ApplyEffectToActorsWithinRadius();
	UFUNCTION()
	virtual void RemoveEffectArea();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<UGameplayEffect> EffectToApplyToOverlapActors;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TSubclassOf<UGameplayEffect> AdditionalEffectToApplyToSelf;
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<UGameplayEffect> EffectToApplyToSelfOnEnd;
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	bool bEndAbilityOnDurationEnd;
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	bool bActivateImmediately = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Duration")
	FScalableFloat Duration;
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Duration")
	TSubclassOf<AApplyEffectZone> AreaActorClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Instant")
	FScalableFloat Radius;
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Instant")
	bool bIgnoreSelf;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|SetByCaller")
	bool bIsSetByCaller;
	UPROPERTY(EditDefaultsOnly, Category = "Ability|SetByCaller")
	FScalableFloat SetByCallerMagnitude;
	UPROPERTY(EditDefaultsOnly, Category = "Ability|SetByCaller")
	FGameplayTag SetByCallerTag;

	UPROPERTY()
	AActor* SpawnedActor;
	
	FTimerHandle TimerHandle;
	FActiveGameplayEffectHandle ActiveSelfEffectHandle;
	FActiveGameplayEffectHandle ActiveEndEffectHandle;
};
