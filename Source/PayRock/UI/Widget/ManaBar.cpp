// PayRockGames


#include "ManaBar.h"
#include "Components/ProgressBar.h"
#include "PayRock/UI/WidgetController/OverlayWidgetController.h"

void UManaBar::OnWidgetControllerSet()
{
	Super::OnWidgetControllerSet();

	if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		OverlayWidgetController->OnManaChanged.AddDynamic(this, &UManaBar::OnManaChanged);
		OverlayWidgetController->OnMaxManaChanged.AddDynamic(this, &UManaBar::OnMaxManaChanged);
	}
}

void UManaBar::OnManaChanged(float NewMana)
{
	Mana = NewMana;
	ManaBar->SetPercent(Mana / (MaxMana == 0 ? 1 : MaxMana));
}

void UManaBar::OnMaxManaChanged(float NewMaxMana)
{
	MaxMana = NewMaxMana;
	ManaBar->SetPercent(Mana / (MaxMana == 0 ? 1 : MaxMana));
}
