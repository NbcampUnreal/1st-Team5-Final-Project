// PayRockGames


#include "PayRock/UI/Widget/Stat/StatTooltipWidget.h"
#include "Components/TextBlock.h"


void UStatTooltipWidget::SetTooltipContent(const FString& InStatName, const FString& InDescription)
{
	if (StatNameText)
	{
		StatNameText->SetText(FText::FromString(InStatName));
	}
	if (DescriptionText)
	{
		DescriptionText->SetText(FText::FromString(InDescription));
	}
}
