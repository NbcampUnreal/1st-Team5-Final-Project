#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Navigation/PathFollowingComponent.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "GA_ClownBuneAttack.generated.h"

UCLASS()
class PAYROCK_API UGA_ClownBuneAttack : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_ClownBuneAttack();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	void TryActivateAttackAbility(class AMarketClownMonster* Boss);
	void SafeEndAbility(bool bWasCancelled);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	TSubclassOf<class UGameplayAbility> WeaponCollisionAbility;

private:
	bool bAbilityEnded = false;
};