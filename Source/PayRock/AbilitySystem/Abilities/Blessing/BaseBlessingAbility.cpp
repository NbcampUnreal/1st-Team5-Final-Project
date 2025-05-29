// PayRockGames


#include "BaseBlessingAbility.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"

void UBaseBlessingAbility::ApplyBlessingEffect()
{
	if (IsValid(BlessingEffectClass))
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(BlessingEffectClass, GetAbilityLevel());
		bool bIsInstant = SpecHandle.Data->GetDuration() == FGameplayEffectConstants::INSTANT_APPLICATION;
		FActiveGameplayEffectHandle ActiveEffectHandle = ApplyGameplayEffectSpecToOwner(
			GetCurrentAbilitySpecHandle(),
			GetCurrentActorInfo(),
			GetCurrentActivationInfo(),
			SpecHandle	
			);

		if (bIsInstant)
		{
			ApplyPenaltyEffectOrAbility(FGameplayEffectRemovalInfo());
			return;
		}
	
		auto OnEffectRemoved =
			GetAbilitySystemComponentFromActorInfo()->OnGameplayEffectRemoved_InfoDelegate(ActiveEffectHandle);
		if (OnEffectRemoved)
		{
			OnEffectRemoved->AddUObject(this, &UBaseBlessingAbility::ApplyPenaltyEffectOrAbility);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("OnEffectRemoved delegate is not valid. Penalty won't apply for this blessing."));
		}
	}
}

void UBaseBlessingAbility::ApplyPenaltyEffectOrAbility(const FGameplayEffectRemovalInfo& RemovalInfo)
{
	for (const auto PenaltyEffectClass : PenaltyEffectClasses)
	{
		if (IsValid(PenaltyEffectClass))
		{
			FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(PenaltyEffectClass, GetAbilityLevel());
			ActivePenaltyEffectHandle = ApplyGameplayEffectSpecToOwner(
			GetCurrentAbilitySpecHandle(),
			GetCurrentActorInfo(),
			GetCurrentActivationInfo(),
			SpecHandle	
			);
		}
	}

	if (IsValid(PenaltyAbilityClass))
	{
		if (UPRAbilitySystemComponent* ASC = Cast<UPRAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
		{
			const FGameplayAbilityActivationInfo& ActivationInfo = GetCurrentActivationInfoRef();
			if (ASC->HasAuthorityOrPredictionKey(&ActivationInfo))
			{
				ASC->AddAbility(PenaltyAbilityClass, true, GetAbilityLevel());
			}
		}
	}
}

