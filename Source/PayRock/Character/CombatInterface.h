// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "CombatInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};


class PAYROCK_API ICombatInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& SocketTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat|Sound")
	USoundAttenuation* GetHitSoundAttenuation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat|Sound")
	USoundBase* GetHitSound() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat|Sound")
	USoundBase* GetLocalOnlyHitSound() const;
};
