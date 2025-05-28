// PayRockGames


#include "BlessingContainerWidget.h"
#include "BlessingRowWidget.h"
#include "Components/ScrollBox.h"
#include "PayRock/Character/Blessing/BlessingData.h"
#include "PayRock/UI/WidgetController/BlessingWidgetController.h"

void UBlessingContainerWidget::OnWidgetControllerSet()
{
	InitializeBlessingRows();
}

void UBlessingContainerWidget::InitializeBlessingRows()
{
	UBlessingWidgetController* BlessingWidgetController = Cast<UBlessingWidgetController>(WidgetController);
	if (!BlessingWidgetController) return;
	
	for (auto Blessing : BlessingWidgetController->BlessingsContainer)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(this, BlessingRowWidgetClass);
		UBlessingRowWidget* RowWidget = Cast<UBlessingRowWidget>(Widget);
		if (!RowWidget) return;

		switch (Blessing.BlessingType)
		{
		case EBlessingType::Active:
			ActiveBlessingBox->AddChild(RowWidget);
			break;

		case EBlessingType::Passive:
			PassiveBlessingBox->AddChild(RowWidget);
			break;
		}
		
		RowWidget->SetBlessingName(Blessing.KoreanName);
		RowWidget->SetBlessingImage(Blessing.Icon);
		FText InfoText = FText::Format(
			FText::FromString("- Ability: {0}\n- Penalty: {1}"),
			Blessing.BlessingDescription,
			Blessing.PenaltyDescription
		);
		RowWidget->SetInfoText(InfoText);
		RowWidget->SetBlessing(Blessing);
		RowWidget->SetWidgetController(BlessingWidgetController);
	}
}
