// PayRockGames


#include "StatContainerWidget.h"
#include "StatRowWidget.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "PayRock/UI/WidgetController/StatWidgetController.h"

void UStatContainerWidget::OnWidgetControllerSet()
{
	if (UStatWidgetController* StatWidgetController = Cast<UStatWidgetController>(WidgetController))
	{
		InitializeStatRows();
		StatWidgetController->OnAttributeChangeDelegate.AddUniqueDynamic(this, &UStatContainerWidget::UpdateStatRow);
	}
}

void UStatContainerWidget::InitializeStatRows()
{
	UStatWidgetController* StatWidgetController = Cast<UStatWidgetController>(WidgetController);
	if (!StatWidgetController) return;
	
	for (const FString& AttributeName : StatWidgetController->AttributeNames)
	{
		UStatRowWidget* StatRowWidget = Cast<UStatRowWidget>(
			CreateWidget<UUserWidget>(this, StatRowWidgetClass)
			);
		StatRowWidget->SetStatName(AttributeName);
		
		/*
		 * TODO: This is STUPID!!! Separate Primary / Secondary using GameplayTag?
		 */
		if (PrimaryStatNames.Contains(AttributeName))
		{
			PrimaryStatBox->AddChildToVerticalBox(StatRowWidget);
		}
		else
		{
			SecondaryStatBox->AddChild(StatRowWidget);
		}
		
		StatNameRowMap.Add(AttributeName, StatRowWidget);
	}
}

void UStatContainerWidget::UpdateStatRow(const FString& StatName, const float& StatValue)
{
	if (UUserWidget** FoundWidgetPtr = StatNameRowMap.Find(StatName))
	{
		if (UStatRowWidget* StatRowWidget = Cast<UStatRowWidget>(*FoundWidgetPtr))
		{
			StatRowWidget->SetStatValue(StatValue);
		}
	}
}

