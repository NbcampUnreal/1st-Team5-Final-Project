// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseDamageGameplayAbility.h"
#include "BaseMontageDamageSkill.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBaseMontageDamageSkill : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	virtual void OnMontageEnd();
	UFUNCTION()
	virtual void OnMontageInterrupted();
	UFUNCTION()
	virtual void OnEventReceived(FGameplayEventData Payload);
	
	/* Montage */
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TObjectPtr<UAnimMontage> ActivationMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	bool bLockInputDuringMontage = true;
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	bool bEndAbilityOnMontageEnd = false;
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	bool bEndAbilityOnMontageInterrupted = false;
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	float MontageRate = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	FGameplayTag TriggerTag;
	UPROPERTY()
	TObjectPtr<APlayerController> CachedController = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<UGameplayEffect> InputBlockEffect;
	
};
