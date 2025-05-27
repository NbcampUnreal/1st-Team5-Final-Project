// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "BaseBlessingAbility.generated.h"


UCLASS()
class PAYROCK_API UBaseBlessingAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void ApplyBlessingEffect();

	UFUNCTION()
	void ApplyPenaltyEffect(const FGameplayEffectRemovalInfo& RemovalInfo);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blessing")
	TSubclassOf<UGameplayEffect> BlessingEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blessing")
	TSubclassOf<UGameplayEffect> PenaltyEffectClass;

	UPROPERTY(BlueprintReadOnly, Category = "Blessing")
	FActiveGameplayEffectHandle ActivePenaltyEffectHandle;
	
};
