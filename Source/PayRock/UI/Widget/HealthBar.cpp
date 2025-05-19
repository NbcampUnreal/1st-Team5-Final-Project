// PayRockGames


#include "HealthBar.h"
#include "Components/ProgressBar.h"
#include "PayRock/UI/WidgetController/OverlayWidgetController.h"

void UHealthBar::OnWidgetControllerSet()
{
	Super::OnWidgetControllerSet();

	if (UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		OverlayWidgetController->OnHealthChanged.AddDynamic(this, &UHealthBar::OnHealthChanged);
		OverlayWidgetController->OnMaxHealthChanged.AddDynamic(this, &UHealthBar::OnMaxHealthChanged);
	}
}

void UHealthBar::OnHealthChanged(float NewHealth)
{
	Health = NewHealth;
	HealthBar->SetPercent(Health / (MaxHealth == 0 ? 1 : MaxHealth));
}

void UHealthBar::OnMaxHealthChanged(float NewMaxHealth)
{
	MaxHealth = NewMaxHealth;
	HealthBar->SetPercent(Health / (MaxHealth == 0 ? 1 : MaxHealth));
}
