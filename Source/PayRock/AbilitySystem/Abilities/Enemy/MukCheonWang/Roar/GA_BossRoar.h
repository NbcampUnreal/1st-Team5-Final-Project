// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_BossRoar.generated.h"

class UAnimMontage;

UCLASS()
class PAYROCK_API UGA_BossRoar : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_BossRoar();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Roar")
	TObjectPtr<UAnimMontage> RoarMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Roar")
	FGameplayTag ActiveRoarTag;
	
	UFUNCTION()
	void OnRoarMontageEnded();

	UFUNCTION()
	void OnRoarMontageCancelled();

private:
	FGameplayAbilitySpecHandle CurrentSpecHandle;
	FGameplayAbilityActorInfo* CurrentActorInfo = nullptr;
	FGameplayAbilityActivationInfo CurrentActivationInfo;
};
