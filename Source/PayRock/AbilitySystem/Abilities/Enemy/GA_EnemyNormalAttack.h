// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "PayRock/Character/BaseCharacter.h"
#include "GA_EnemyNormalAttack.generated.h"

class ASamuraiStatueCharacter;
/**
 * 
 */
UCLASS()
class PAYROCK_API UGA_EnemyNormalAttack : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()


public:
	UGA_EnemyNormalAttack();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	

private:
	UFUNCTION(BlueprintCallable)
	void EndAttackBlackboardState();

	UPROPERTY()
	ASamuraiStatueCharacter* samurai;
	bool bHit = false;

	
};
