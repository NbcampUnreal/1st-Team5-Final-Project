// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "GA_BossAttackSelector.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UGA_BossAttackSelector : public UBaseGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_BossAttackSelector();

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boss Attack")
	TArray<FGameplayTag> AttackTags;
};
