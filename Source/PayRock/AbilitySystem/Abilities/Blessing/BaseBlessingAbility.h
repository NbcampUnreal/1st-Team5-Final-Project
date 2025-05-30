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
	void ApplyPenaltyEffectOrAbility(const FGameplayEffectRemovalInfo& RemovalInfo);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blessing")
	TSubclassOf<UGameplayEffect> BlessingEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blessing")
	TArray<TSubclassOf<UGameplayEffect>> PenaltyEffectClasses;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blessing")
	TSubclassOf<UGameplayAbility> PenaltyAbilityClass;

	UPROPERTY(BlueprintReadOnly, Category = "Blessing")
	FActiveGameplayEffectHandle ActivePenaltyEffectHandle;
	
};
