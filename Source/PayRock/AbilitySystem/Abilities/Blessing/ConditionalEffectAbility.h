// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "GameplayEffectTypes.h"
#include "ConditionalEffectAbility.generated.h"

UENUM(BlueprintType)
enum class EAttributeComparisonType : uint8
{
	LessThan,
	LessThanOrEqual,
	GreaterThan,
	GreaterThanOrEqual
};

UCLASS()
class PAYROCK_API UConditionalEffectAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Conditional Effect")
	TSubclassOf<UGameplayEffect> ConditionalEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Conditional Effect")
	FScalableFloat ThresholdPercent;

	UPROPERTY(EditDefaultsOnly, Category = "Conditional Effect")
	EAttributeComparisonType ComparisonType = EAttributeComparisonType::LessThan;

	UPROPERTY(EditDefaultsOnly, Category = "Conditional Effect")
	FGameplayAttribute CurrentAttribute;

	UPROPERTY(EditDefaultsOnly, Category = "Conditional Effect")
	FGameplayAttribute MaxAttribute;

private:
	void EvaluateAndApplyOrRemoveEffect(const FOnAttributeChangeData& Data);
	bool DoesConditionPass(float Current, float Max) const;
	void FinishWait();
	
	FDelegateHandle CurrentAttributeDelegateHandle;
	FDelegateHandle MaxAttributeDelegateHandle;

	FActiveGameplayEffectHandle ActiveEffectHandle;

	FTimerHandle WaitTimerHandle;

	bool bShouldWait = false;
	
};
