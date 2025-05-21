// PayRockGames


#include "StatWindowWidget.h"

void UStatWindowWidget::OnWidgetControllerSet()
{
	if (StatContainer)
	{
		StatContainer->SetWidgetController(WidgetController);
	}
}
