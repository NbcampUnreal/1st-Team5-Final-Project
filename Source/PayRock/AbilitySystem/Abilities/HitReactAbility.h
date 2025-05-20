// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "HitReactAbility.generated.h"

UCLASS()
class PAYROCK_API UHitReactAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ApplyHitReactEffectToOwner();

private:
	UPROPERTY(EditDefaultsOnly, Category = "HitReact")
	TSubclassOf<UGameplayEffect> HitReactEffect;
};
