// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CleanPercent.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UCleanPercent : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetCleanData();

private:
	FName CleanPercent = "CleanPercent";
	FName CleanTextMessage = "CleanMessage";
};
