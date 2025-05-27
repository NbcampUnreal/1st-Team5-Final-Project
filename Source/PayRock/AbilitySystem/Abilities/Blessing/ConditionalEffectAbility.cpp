// PayRockGames


#include "ConditionalEffectAbility.h"
#include "AbilitySystemComponent.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"
#include "PayRock/Character/BaseCharacter.h"


void UConditionalEffectAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	if (CurrentAttribute.IsValid())
	{
		CurrentAttributeDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(CurrentAttribute)
			.AddUObject(this, &UConditionalEffectAbility::EvaluateAndApplyOrRemoveEffect);
	}

	if (MaxAttribute.IsValid())
	{
		MaxAttributeDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(MaxAttribute)
			.AddUObject(this, &UConditionalEffectAbility::EvaluateAndApplyOrRemoveEffect);
	}
	
	EvaluateAndApplyOrRemoveEffect(FOnAttributeChangeData());
}

void UConditionalEffectAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	if (CurrentAttributeDelegateHandle.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(CurrentAttribute)
			.Remove(CurrentAttributeDelegateHandle);
	}

	if (MaxAttributeDelegateHandle.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(MaxAttribute)
			.Remove(MaxAttributeDelegateHandle);
	}

	if (ActiveEffectHandle.IsValid())
	{
		ASC->RemoveActiveGameplayEffect(ActiveEffectHandle);

		if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetAbilitySystemComponentFromActorInfo()->GetAvatarActor()))
		{
			Character->ApplySecondaryAttributeInitEffect();
		}
	}
}

void UConditionalEffectAbility::EvaluateAndApplyOrRemoveEffect(const FOnAttributeChangeData& Data)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC || !ConditionalEffect) return;
	const UAttributeSet* AS = ASC->GetAttributeSet(UPRAttributeSet::StaticClass());
	if (!AS) return;
	
	float Current = CurrentAttribute.GetNumericValue(AS);
	float Max = MaxAttribute.GetNumericValue(AS);

	if (DoesConditionPass(Current, Max))
	{
		if (!ActiveEffectHandle.IsValid())
		{
			FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(ConditionalEffect, GetAbilityLevel());
			ActiveEffectHandle = ApplyGameplayEffectSpecToOwner(
				GetCurrentAbilitySpecHandle(),
				GetCurrentActorInfo(),
				GetCurrentActivationInfo(),
				SpecHandle);

			if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetAbilitySystemComponentFromActorInfo()->GetAvatarActor()))
			{
				Character->ApplySecondaryAttributeInitEffect();
			}
		}
	}
	else
	{
		if (ActiveEffectHandle.IsValid())
		{
			ASC->RemoveActiveGameplayEffect(ActiveEffectHandle);
			ActiveEffectHandle.Invalidate();

			if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetAbilitySystemComponentFromActorInfo()->GetAvatarActor()))
			{
				Character->ApplySecondaryAttributeInitEffect();
			}
		}
	}
}

bool UConditionalEffectAbility::DoesConditionPass(float Current, float Max) const
{
	const float Threshold = ThresholdPercent.GetValueAtLevel(GetAbilityLevel()) / 100.f * Max;

	switch (ComparisonType)
	{
	case EAttributeComparisonType::LessThan:
		return Current < Threshold;
	case EAttributeComparisonType::LessThanOrEqual:
		return Current <= Threshold;
	case EAttributeComparisonType::GreaterThan:
		return Current > Threshold;
	case EAttributeComparisonType::GreaterThanOrEqual:
		return Current >= Threshold;
	default:
		return false;
	}
}
