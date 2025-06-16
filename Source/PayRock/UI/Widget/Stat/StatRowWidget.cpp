// PayRockGames


#include "StatRowWidget.h"
#include "StatTooltipWidget.h"
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



void UStatRowWidget::SetStatDescription(const FString& InDescription, const FText& InDisplayName)
{
	StatDescription = InDescription;
	StatDisplayName = InDisplayName;

	if (!TooltipWidgetClass) return;

	UUserWidget* TooltipWidget = CreateWidget<UUserWidget>(this, TooltipWidgetClass);
	if (!TooltipWidget) return;

	if (UStatTooltipWidget* StatTooltip = Cast<UStatTooltipWidget>(TooltipWidget))
	{
		// 여기서 한글 이름 전달!
		StatTooltip->SetTooltipContent(StatDisplayName.ToString(), StatDescription);
	}

	SetToolTip(TooltipWidget);
}

//void UStatRowWidget::SetStatDescription(const FString& InDescription)
//{
//	StatDescription = InDescription;
//
//	if (!TooltipWidgetClass) return;
//
//	UUserWidget* TooltipWidget = CreateWidget<UUserWidget>(this, TooltipWidgetClass);
//	if (!TooltipWidget) return;
//
//	if (UStatTooltipWidget* StatTooltip = Cast<UStatTooltipWidget>(TooltipWidget))
//	{
//		StatTooltip->SetTooltipContent(StatName->GetText().ToString(), StatDescription);
//	}
//
//	this->SetToolTip(TooltipWidget); // 반드시 this
//}
