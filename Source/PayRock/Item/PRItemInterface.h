// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PayRock/AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "PRItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UPRItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PAYROCK_API IPRItemInterface
{
	GENERATED_BODY()

public:

	virtual FString GetName() const = 0;
	
	virtual TSubclassOf<UGameplayAbility> GetAbilities() const = 0;

	virtual int32 GetAbilityLevel() const = 0;

	virtual int32 GetItemPrice() const = 0;
};
