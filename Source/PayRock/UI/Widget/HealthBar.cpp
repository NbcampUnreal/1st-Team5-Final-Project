// PayRockGames


#include "HealthBar.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
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
	UpdateHeartIcon();
}

void UHealthBar::OnMaxHealthChanged(float NewMaxHealth)
{
	MaxHealth = NewMaxHealth;
	HealthBar->SetPercent(Health / (MaxHealth == 0 ? 1 : MaxHealth));
	UpdateHeartIcon();
}

void UHealthBar::UpdateHeartIcon()
{
	// 심장이미지 없거나 풀피가 0이라면 리턴
	if (!HeartImage || MaxHealth == 0) return;

	float HealthPercent = Health / MaxHealth * 100.0f;

	UMaterialInterface* MaterialToApply = nullptr;

	if (HealthPercent > 75.f)
	{
		MaterialToApply = HeartMatFull;
	}
	else if (HealthPercent > 50.f)
	{
		MaterialToApply = HeartMat3Quarter;
	}
	else if (HealthPercent > 25.f)
	{
		MaterialToApply = HeartMatHalf;
	}
	else
	{
		MaterialToApply = HeartMatEmpty;
	}

	if (MaterialToApply)
	{
		HeartImage->SetBrushFromMaterial(MaterialToApply);
	}
}
