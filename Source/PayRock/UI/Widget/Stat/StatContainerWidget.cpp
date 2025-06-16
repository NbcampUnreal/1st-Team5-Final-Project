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

	//  기존 내용 정리
	PrimaryStatBox->ClearChildren();
	SecondaryStatBox->ClearChildren();
	StatNameRowMap.Empty();
	StatDescriptions.Empty();

	//  에셋에서 설명 불러오기
	if (StatInfoDataAsset)
	{
		for (const FStatInfo& Info : StatInfoDataAsset->StatList)
		{
			StatDescriptions.Add(Info.StatName, Info.Description.ToString());
		}
	}

	//  위젯 생성 & 설명 연결
	for (const FString& AttributeName : StatWidgetController->AttributeNames)
	{
		UStatRowWidget* StatRowWidget = Cast<UStatRowWidget>(
			CreateWidget<UUserWidget>(this, StatRowWidgetClass)
		);
		if (!StatRowWidget) continue;

		// 이름 설정
		StatRowWidget->SetStatName(AttributeName);

		//  DataAsset에서 DisplayName, Description 가져오기
		if (const FStatInfo* StatInfo = StatInfoDataAsset->StatInfoMap.Find(FName(*AttributeName)))
		{
			// FText + FString 함께 전달
			StatRowWidget->SetStatDescription(StatInfo->Description.ToString(), StatInfo->StatDisplayName);
		}

		// 기본 위치 배치
		if (PrimaryStatNames.Contains(AttributeName))
		{
			PrimaryStatBox->AddChildToVerticalBox(StatRowWidget);
		}
		else
		{
			SecondaryStatBox->AddChild(StatRowWidget);
		}

		// 맵에 저장
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

