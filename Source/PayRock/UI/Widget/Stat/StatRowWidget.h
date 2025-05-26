// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatRowWidget.generated.h"

class UTextBlock;

UCLASS(meta = (DisableNativeTick))
class PAYROCK_API UStatRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetStatName(const FString& Name);
	void SetStatValue(const float& Value);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatName;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatValue;
};
