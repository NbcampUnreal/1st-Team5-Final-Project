// PayRockGames


#include "StatRowWidget.h"
#include "Components/TextBlock.h"

void UStatRowWidget::SetStatName(const FString& Name)
{
	if (StatName)
	{
		StatName->SetText(FText::FromString(Name));
	}
}

void UStatRowWidget::SetStatValue(const float& Value)
{
	if (StatValue)
	{
		StatValue->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), Value)));
	}
}
