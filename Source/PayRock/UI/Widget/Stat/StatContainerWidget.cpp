// PayRockGames


#include "StatContainerWidget.h"
#include "StatRowWidget.h"
#include "StatInfoDataAsset.h"
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

	PrimaryStatBox->ClearChildren();
	SecondaryStatBox->ClearChildren();
	StatNameRowMap.Empty();
	StatDescriptions.Empty();

	if (StatInfoDataAsset)
	{
		for (const FStatInfo& Info : StatInfoDataAsset->StatList)
		{
			StatDescriptions.Add(Info.StatName, Info.Description.ToString());
		}
	}

	for (const FString& AttributeName : StatWidgetController->AttributeNames)
	{
		UStatRowWidget* StatRowWidget = Cast<UStatRowWidget>(
			CreateWidget<UUserWidget>(this, StatRowWidgetClass)
		);
		if (!StatRowWidget) continue;

		StatRowWidget->SetStatName(AttributeName);

		// =======
		if (StatInfoDataAsset)
		{
			if (const FStatInfo* StatInfo = StatInfoDataAsset->StatInfoMap.Find(FName(*AttributeName)))
			{
				StatRowWidget->SetStatDescription(StatInfo->Description.ToString(), StatInfo->StatDisplayName);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("StatInfoMap [%s]"), *AttributeName);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("StatInfoDataAsset Error!"));
		}
		// ============
		
		if (const FStatInfo* StatInfo = StatInfoDataAsset->StatInfoMap.Find(FName(*AttributeName)))
		{
			StatRowWidget->SetStatDescription(StatInfo->Description.ToString(), StatInfo->StatDisplayName);
		}
		
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

