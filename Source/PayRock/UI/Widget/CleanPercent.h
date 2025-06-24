// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseUserWidget.h"
#include "CleanPercent.generated.h"


UCLASS()
class PAYROCK_API UCleanPercent : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetCleanData();

private:
	FName CleanPercent = "CleanPercent";
	FName CleanTextMessage = "CleanMessage";
};
