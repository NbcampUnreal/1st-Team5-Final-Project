
#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "PayRock/Enemy/SpecialEnemy/MarketClown/MarketClownMonster.h"
#include "GA_ClownAttack.generated.h"

UCLASS()
class PAYROCK_API UGA_ClownAttack : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_ClownAttack();
	
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

	UPROPERTY(EditDefaultsOnly, Category = "Cooldown")
	TSubclassOf<UGameplayEffect> CooldownGameplayEffect;

private:
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void ResetBlackboardAttackState(AMarketClownMonster* Boss);
};

