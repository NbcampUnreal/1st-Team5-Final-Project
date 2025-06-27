// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "GA_BossMeleeAttack_Base.generated.h"

class AMukCheonWangCharacter;
/**
 * 
 */
UCLASS()
class PAYROCK_API UGA_BossMeleeAttack_Base : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()
	
	
public:
	UGA_BossMeleeAttack_Base();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION(BlueprintCallable)
	void EndAttackBlackboardState();

	UPROPERTY()
	AMukCheonWangCharacter* BossCharacter = nullptr;

	bool bHit = false;
};
