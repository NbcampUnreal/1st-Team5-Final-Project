// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "GA_ClownImeaAttack.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UGA_ClownImeaAttack : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_ClownImeaAttack();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnWeaponAbilityEnded(UGameplayAbility* Ability);
	void SafeEndAbility(bool bWasCancelled);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	TSubclassOf<UGameplayAbility> WeaponAttackAbility;

private:
	bool bAbilityEnded = false;
};