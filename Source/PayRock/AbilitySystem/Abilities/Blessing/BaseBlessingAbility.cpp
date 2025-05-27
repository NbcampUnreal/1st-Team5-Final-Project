// PayRockGames


#include "BaseBlessingAbility.h"
#include "AbilitySystemComponent.h"

void UBaseBlessingAbility::ApplyBlessingEffect()
{
	if (!IsValid(BlessingEffectClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("BlessingEffectClass is not valid."));
		return;
	}
	
	FActiveGameplayEffectHandle ActiveEffectHandle = ApplyGameplayEffectSpecToOwner(
		GetCurrentAbilitySpecHandle(),
		GetCurrentActorInfo(),
		GetCurrentActivationInfo(),
		MakeOutgoingGameplayEffectSpec(BlessingEffectClass, GetAbilityLevel())	
		);

	auto OnEffectRemoved =
	GetAbilitySystemComponentFromActorInfo()->OnGameplayEffectRemoved_InfoDelegate(ActiveEffectHandle);
	if (OnEffectRemoved)
	{
		OnEffectRemoved->AddUObject(this, &UBaseBlessingAbility::ApplyPenaltyEffect);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnEffectRemoved delegate is not valid. Penalty won't apply for this blessing."));
	}
}

void UBaseBlessingAbility::ApplyPenaltyEffect(const FGameplayEffectRemovalInfo& RemovalInfo)
{
	if (!IsValid(PenaltyEffectClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("PenaltyEffectClass is not valid."));
		return;
	}
	
	ActivePenaltyEffectHandle = ApplyGameplayEffectSpecToOwner(
		GetCurrentAbilitySpecHandle(),
		GetCurrentActorInfo(),
		GetCurrentActivationInfo(),
		MakeOutgoingGameplayEffectSpec(PenaltyEffectClass, GetAbilityLevel())	
		);
}
