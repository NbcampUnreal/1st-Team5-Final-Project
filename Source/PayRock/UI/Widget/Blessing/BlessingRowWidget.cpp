// PayRockGames

#include "BlessingRowWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "PayRock/UI/WidgetController/BlessingWidgetController.h"

void UBlessingRowWidget::OnWidgetControllerSet()
{
	if (!IsValid(OverlayButton)) return;

	OverlayButton->OnClicked.AddUniqueDynamic(this, &UBlessingRowWidget::NotifyBlessingSelected);
}

void UBlessingRowWidget::NotifyBlessingSelected()
{
	if (UBlessingWidgetController* WC = Cast<UBlessingWidgetController>(WidgetController))
	{
		WC->HandleBlessingSelection(CachedBlessing);
	}
}

void UBlessingRowWidget::SetBlessingImage(UTexture2D* ImageTexture)
{
	if (!IsValid(BlessingImage) || !IsValid(ImageTexture)) return;

	BlessingImage->SetBrushFromTexture(ImageTexture);
}

void UBlessingRowWidget::SetBlessingName(const FText& Text)
{
	if (!IsValid(BlessingName)) return;

	BlessingName->SetText(Text);
}

void UBlessingRowWidget::SetBlessing(const FBlessingData& Blessing)
{
	CachedBlessing = Blessing;
}

void UBlessingRowWidget::SetInfoText(const FText& Text)
{
	SetToolTipText(Text);
}
