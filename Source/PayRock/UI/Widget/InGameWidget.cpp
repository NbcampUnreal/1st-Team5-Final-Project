// PayRockGames


#include "InGameWidget.h"

void UInGameWidget::OnWidgetControllerSet()
{
	Super::OnWidgetControllerSet();

	HealthBar->SetWidgetController(WidgetController);
	// ManaBar->SetWidgetController(WidgetController);
}
