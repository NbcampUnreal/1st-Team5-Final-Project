// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PRItemInterface.generated.h"

class UGameplayAbility;

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
	virtual void UsingEffect() = 0;

	virtual FString GetName() const = 0;
	
	virtual TSubclassOf<UGameplayAbility> GetAbilities() const = 0;

	virtual float GetAbilityLevel() const = 0;

	virtual int32 GetItemPrice() const = 0;
};
