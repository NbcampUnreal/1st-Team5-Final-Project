// PayRockGames


#include "BossWeaponSkill.h"
#include "AbilitySystemComponent.h"

void UBossWeaponSkill::OnEventReceived(FGameplayEventData Payload)
{
	Super::OnEventReceived(Payload);

	int32 RandomIndex = FMath::RandRange(0, ActivatableAbilities.Num() - 1);
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(ActivatableAbilities[RandomIndex], 1);
	ASC->GiveAbilityAndActivateOnce(AbilitySpec);
}
