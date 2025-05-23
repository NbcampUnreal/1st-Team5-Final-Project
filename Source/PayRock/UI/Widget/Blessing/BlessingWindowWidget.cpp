// PayRockGames


#include "BlessingWindowWidget.h"

void UBlessingWindowWidget::OnWidgetControllerSet()
{
	if (EquippedBlessingWidget)
	{
		EquippedBlessingWidget->SetWidgetController(WidgetController);
	}

	if (BlessingContainerWidget)
	{
		BlessingContainerWidget->SetWidgetController(WidgetController);
	}
}
