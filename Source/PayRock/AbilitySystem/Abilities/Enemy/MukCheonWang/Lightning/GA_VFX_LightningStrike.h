// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "GA_VFX_LightningStrike.generated.h"


class ALightningStrikeActor;

UCLASS()
class PAYROCK_API UGA_VFX_LightningStrike : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_VFX_LightningStrike();

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, Category = "Lightning")
	TSubclassOf<ALightningStrikeActor> LightningClass;

	UPROPERTY(EditDefaultsOnly, Category = "Lightning")
	float PredictDelay = 1.5f;
};