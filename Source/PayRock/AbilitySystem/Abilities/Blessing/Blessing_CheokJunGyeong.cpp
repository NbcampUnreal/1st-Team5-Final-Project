// PayRockGames


#include "Blessing_CheokJunGyeong.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"


void UBlessing_CheokJunGyeong::OnStartEventReceived(FGameplayEventData Payload)
{
	FGameplayAbilityActivationInfo ActivationInfo = GetCurrentActivationInfoRef();
	if (HasAuthorityOrPredictionKey(GetCurrentActorInfo(), &ActivationInfo))
	{
		if (IsValid(AdditionalEffectToApplyToSelf))
		{
			FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(
				AdditionalEffectToApplyToSelf, GetAbilityLevel());
			ActiveSelfEffectHandle = ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(),
				GetCurrentActorInfo(), GetCurrentActivationInfo(), SpecHandle);
		}
		
		SpawnEffectArea();
	}
}
