// PayRockGames


#include "ConditionalEffectAbility.h"
#include "AbilitySystemComponent.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"


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

		/*if (GetAvatarActorFromActorInfo()->HasAuthority())
		{
			if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo()))
			{
				Character->RecalculateSecondaryAttributes();
			}
		}*/
	}
}

void UConditionalEffectAbility::EvaluateAndApplyOrRemoveEffect(const FOnAttributeChangeData& Data)
{
	if (bShouldWait || !GetAvatarActorFromActorInfo()->HasAuthority()) return;
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC || !ConditionalEffect) return;
	const UAttributeSet* AS = ASC->GetAttributeSet(UPRAttributeSet::StaticClass());
	if (!AS) return;
	
	const float Current = CurrentAttribute.GetNumericValue(AS);
	const float Max = MaxAttribute.GetNumericValue(AS);

	const bool bDoesConditionPass = DoesConditionPass(Current, Max);
	
	if (bDoesConditionPass)
	{
		bShouldWait = true;
		if (!ActiveEffectHandle.IsValid())
		{
			FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(ConditionalEffect, GetAbilityLevel());
			ActiveEffectHandle = ApplyGameplayEffectSpecToOwner(
				GetCurrentAbilitySpecHandle(),
				GetCurrentActorInfo(),
				GetCurrentActivationInfo(),
				SpecHandle);
			UE_LOG(LogTemp, Warning, TEXT("ConditionalEffect was applied"))
		
			/*if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo()))
			{
				Character->RecalculateSecondaryAttributesDelayed();
				UE_LOG(LogTemp, Warning, TEXT("Recalculated Secondary Attributes"))
			}*/
		}
	}
	else
	{
		bShouldWait = true;
		if (ActiveEffectHandle.IsValid())
		{
			ASC->RemoveActiveGameplayEffect(ActiveEffectHandle);
			ActiveEffectHandle.Invalidate();
			UE_LOG(LogTemp, Warning, TEXT("ConditionalEffect was removed"))

			/*if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo()))
			{
				Character->RecalculateSecondaryAttributesDelayed();
				UE_LOG(LogTemp, Warning, TEXT("Recalculated Secondary Attributes"))
			}*/
		}
	}

	GetWorld()->GetTimerManager().SetTimer(
		WaitTimerHandle,
		this,
		&UConditionalEffectAbility::FinishWait,
		0.1f
	);
}

bool UConditionalEffectAbility::DoesConditionPass(float Current, float Max) const
{
	const float Threshold = ThresholdPercent.GetValueAtLevel(GetAbilityLevel()) / 100.f * Max;
	const float Buffer = 0.01f * Max;
	

	switch (ComparisonType)
	{
	case EAttributeComparisonType::LessThan:
		return Current < Threshold - Buffer;
	case EAttributeComparisonType::LessThanOrEqual:
		return Current <= Threshold - Buffer;
	case EAttributeComparisonType::GreaterThan:
		return Current > Threshold + Buffer;
	case EAttributeComparisonType::GreaterThanOrEqual:
		return Current >= Threshold + Buffer;
	default:
		return false;
	}
}

void UConditionalEffectAbility::FinishWait()
{
	bShouldWait = false;
}
