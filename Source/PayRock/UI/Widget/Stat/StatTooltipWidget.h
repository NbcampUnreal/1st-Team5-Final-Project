// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatTooltipWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PAYROCK_API UStatTooltipWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetTooltipContent(const FString& InStatName, const FString& InDescription);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StatNameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DescriptionText;

};
