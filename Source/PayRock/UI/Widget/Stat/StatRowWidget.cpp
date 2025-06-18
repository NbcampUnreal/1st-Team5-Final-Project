// PayRockGames


#include "StatRowWidget.h"
#include "Components/TextBlock.h"
#include "PayRock/UI/Manager/UIManager.h"

void UStatRowWidget::SetStatName(const FString& Name)
{
	if (StatName)
	{
		FString DisplayString = Name;
		if (UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>())
		{
			DisplayString = UIManager->TranslateEnglishToKorean(DisplayString);
		}
		StatName->SetText(FText::FromString(DisplayString));
	}
}

void UStatRowWidget::SetStatValue(const float& Value)
{
	if (StatValue)
	{
		StatValue->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), Value)));
	}
}
