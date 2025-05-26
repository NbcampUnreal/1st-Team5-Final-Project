// PayRockGames


#include "EquippedBlessingWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "PayRock/UI/WidgetController/BlessingWidgetController.h"

void UEquippedBlessingWidget::OnWidgetControllerSet()
{
	UBlessingWidgetController* WC = Cast<UBlessingWidgetController>(WidgetController);
	if (!WC) return;
	
	WC->OnActiveBlessingChange.AddUniqueDynamic(this, &UEquippedBlessingWidget::UpdateActiveBlessing);
	WC->OnPassiveBlessingChange.AddUniqueDynamic(this, &UEquippedBlessingWidget::UpdatePassiveBlessing);
	ActiveRemoveButton->OnClicked.AddUniqueDynamic(this, &UEquippedBlessingWidget::OnActiveRemoveClicked);
	PassiveRemoveButton->OnClicked.AddUniqueDynamic(this, &UEquippedBlessingWidget::OnPassiveRemoveClicked);
}

void UEquippedBlessingWidget::UpdateActiveBlessing(const FBlessingData& Blessing)
{
	if (ActiveBlessingBox)
	{
		ActiveBlessingBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (ActiveBlessingImage)
	{
		ActiveBlessingImage->SetBrushFromTexture(Blessing.Icon);
	}

	if (ActiveBlessingName)
	{
		ActiveBlessingName->SetText(Blessing.BlessingName);
	}

	if (ActiveBlessingDescription)
	{
		ActiveBlessingDescription->SetText(Blessing.BlessingDescription);
	}

	if (PenaltyDescription)
	{
		PenaltyDescription->SetText(Blessing.PenaltyDescription);
	}
}

void UEquippedBlessingWidget::UpdatePassiveBlessing(const FBlessingData& Blessing)
{
	if (PassiveBlessingBox)
	{
		PassiveBlessingBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (PassiveBlessingImage)
	{
		PassiveBlessingImage->SetBrushFromTexture(Blessing.Icon);
	}

	if (PassiveBlessingName)
	{
		PassiveBlessingName->SetText(Blessing.BlessingName);
	}

	if (PassiveBlessingDescription)
	{
		PassiveBlessingDescription->SetText(Blessing.BlessingDescription);
	}
}

void UEquippedBlessingWidget::OnActiveRemoveClicked()
{
	if (!IsValid(ActiveBlessingBox)) return;
	ActiveBlessingBox->SetVisibility(ESlateVisibility::Hidden);
}

void UEquippedBlessingWidget::OnPassiveRemoveClicked()
{
	if (!IsValid(PassiveBlessingBox)) return;
	PassiveBlessingBox->SetVisibility(ESlateVisibility::Hidden);
}
