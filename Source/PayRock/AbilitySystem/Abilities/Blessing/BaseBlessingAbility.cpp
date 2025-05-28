// PayRockGames


#include "BaseBlessingAbility.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"
#include "PayRock/Character/BaseCharacter.h"

void UBaseBlessingAbility::ApplyBlessingEffect()
{
	if (IsValid(BlessingEffectClass))
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(BlessingEffectClass, GetAbilityLevel());
		FActiveGameplayEffectHandle ActiveEffectHandle = ApplyGameplayEffectSpecToOwner(
			GetCurrentAbilitySpecHandle(),
			GetCurrentActorInfo(),
			GetCurrentActivationInfo(),
			SpecHandle	
			);

		// Recalculate secondary attributes in case primary attributes were changed by the GE
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo()))
		{
			Character->RecalculateSecondaryAttributes();
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
	if (IsValid(PenaltyEffectClass))
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(PenaltyEffectClass, GetAbilityLevel());
		ActivePenaltyEffectHandle = ApplyGameplayEffectSpecToOwner(
		GetCurrentAbilitySpecHandle(),
		GetCurrentActorInfo(),
		GetCurrentActivationInfo(),
		SpecHandle	
		);

		// Recalculate secondary attributes in case primary attributes were changed by the GE
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo()))
		{
			Character->RecalculateSecondaryAttributes();
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


