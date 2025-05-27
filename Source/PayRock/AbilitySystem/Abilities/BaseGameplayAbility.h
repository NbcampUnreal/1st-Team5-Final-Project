// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Abilities/GameplayAbility.h"
#include "BaseGameplayAbility.generated.h"

UCLASS()
class PAYROCK_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

protected:
	float GetManaCost(float InLevel = 1.f) const;
	float GetCooldown(float InLevel = 1.f) const;


};
