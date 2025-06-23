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
	if (!GetWorld() || GetWorld()->bIsTearingDown) return;
	
	float OldMana = Mana;
	Mana = NewMana;
	if (!IsValid(ManaBar)) return;
	ManaBar->SetPercent(Mana / (MaxMana == 0 ? 1 : MaxMana));

	// Fade in if Mana decreased by more than 1 and is currently not displayed
	if (Mana < OldMana - 1.f)
	{
		if (!bIsDisplayed && FadeInAnim)
		{
			bIsDisplayed = true;
			PlayAnimation(FadeInAnim);
		}
		GetWorld()->GetTimerManager().SetTimer(FadeOutTimer, this, &UManaBar::FadeOut, DisplayDuration);
	}
}

void UManaBar::OnMaxManaChanged(float NewMaxMana)
{
	MaxMana = NewMaxMana;
	if (!IsValid(ManaBar)) return;
	ManaBar->SetPercent(Mana / (MaxMana == 0 ? 1 : MaxMana));
}

void UManaBar::FadeOut()
{
	if (!FadeOutAnim || !bIsDisplayed) return;
	PlayAnimation(FadeOutAnim);
	bIsDisplayed = false;
}
