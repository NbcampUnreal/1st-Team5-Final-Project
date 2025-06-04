// PayRockGames


#include "Blessing_CheokJunGyeong.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "PayRock/Character/PRCharacter.h"

void UBlessing_CheokJunGyeong::RemoveEffectArea()
{
	Super::RemoveEffectArea();

	bSpawned = false;
	if (APRCharacter* Character = Cast<APRCharacter>(GetAvatarActorFromActorInfo()))
	{
		Character->StopSpin();
	}
}

void UBlessing_CheokJunGyeong::OnStartEventReceived(FGameplayEventData Payload)
{
	if (bSpawned) return;
	bSpawned = true;
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
	if (APRCharacter* Character = Cast<APRCharacter>(GetAvatarActorFromActorInfo()))
	{
		Character->StartSpin();
	}
}
