// PayRockGames

#include "BaseUserWidget.h"

void UBaseUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}

void UBaseUserWidget::OnWidgetControllerSet()
{
}
