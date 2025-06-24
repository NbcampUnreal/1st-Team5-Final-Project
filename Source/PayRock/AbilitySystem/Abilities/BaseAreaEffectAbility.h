// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseGameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
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
	UFUNCTION(BlueprintCallable)
	void SpawnEffectArea();
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToActorsWithinRadius();
	UFUNCTION(BlueprintCallable)
	virtual void RemoveEffectArea();
	UFUNCTION(BlueprintCallable)
	void OnEventReceived(FGameplayEventData Payload);
	UFUNCTION()
	void OnMontageEnded();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<UGameplayEffect> EffectToApplyToOverlapActors;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TSubclassOf<UGameplayEffect> AdditionalEffectToApplyToSelf;
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<UGameplayEffect> EffectToApplyToSelfOnEnd;
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	bool bEndAbilityOnDurationEnd;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	bool bActivateImmediately = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Duration")
	FScalableFloat Duration;
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Duration")
	TSubclassOf<AApplyEffectZone> AreaActorClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Instant")
	FScalableFloat Radius;
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Instant")
	bool bIgnoreSelf;
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Instant")
	bool bIncludeAICharacters;
	UPROPERTY(BlueprintReadOnly, Category = "Ability|Instant")
	TSet<AActor*> UniqueActors;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability|Montage")
	UAnimMontage* ActivationMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability|Montage")
	float MontageRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability|Montage")
	FName MontageStartSection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability|Event")
	FGameplayTag TriggerGameplayTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability|Input")
	bool bLockInputDuringMontage = true;

	UPROPERTY()
	APlayerController* CachedController;

	UPROPERTY()
	UAbilityTask_PlayMontageAndWait* MontageTask;

	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* WaitEventTask;
};
