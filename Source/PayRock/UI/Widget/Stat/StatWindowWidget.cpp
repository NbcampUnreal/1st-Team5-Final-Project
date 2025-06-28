// PayRockGames


#include "StatWindowWidget.h"

void UStatWindowWidget::OnWidgetControllerSet()
{
	if (StatContainer)
	{
		StatContainer->SetWidgetController(WidgetController);
	}
	if (BlessingSkillInfo)
	{
		BlessingSkillInfo->SetWidgetController(WidgetController);
	}
	if (AccessorySkillInfo)
	{
		AccessorySkillInfo->SetWidgetController(WidgetController);
	}
	if (WeaponSkillInfo)
	{
		WeaponSkillInfo->SetWidgetController(WidgetController);
	}
	
}
