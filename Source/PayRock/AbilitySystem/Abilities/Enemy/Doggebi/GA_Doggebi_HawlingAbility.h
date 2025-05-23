// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "PayRock/Enemy/MiddleBoss/Doggebi/DoggebiCharacter.h"
#include "GA_Doggebi_HawlingAbility.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UGA_Doggebi_HawlingAbility : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()

public:
	
	UGA_Doggebi_HawlingAbility();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	
	UPROPERTY()
	ADoggebiCharacter* Doggebi;
};
