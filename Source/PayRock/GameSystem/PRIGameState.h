// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PRIGameState.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPRIGameState : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PAYROCK_API IPRIGameState
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="MatchFlow")
	void Notify_PlayerConnection();
};
