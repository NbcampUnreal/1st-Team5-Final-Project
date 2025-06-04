// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "GA_EnemyDetect.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UGA_EnemyDetect : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_EnemyDetect();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> DetectMontage;

};