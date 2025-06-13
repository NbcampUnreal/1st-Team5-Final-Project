// PayRockGames


#include "BlessingIcon.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "PayRock/PRGameplayTags.h"
#include "PayRock/UI/WidgetController/OverlayWidgetController.h"

void UBlessingIcon::OnWidgetControllerSet()
{
	if (UOverlayWidgetController* OverlayWC = Cast<UOverlayWidgetController>(WidgetController))
	{
		OverlayWC->OnActiveBlessingChanged.AddUniqueDynamic(this, &UBlessingIcon::OnActiveBlessingChanged);

		FOnCooldownChanged& Delegate =
			OverlayWC->CooldownDelegates.FindOrAdd(FPRGameplayTags::Get().Ability_Blessing_Cooldown);
		Delegate.AddUniqueDynamic(this, &UBlessingIcon::UpdateCooldown);
	}
}

void UBlessingIcon::OnActiveBlessingChanged(UTexture2D* EquippedActiveBlessingIcon)
{
	if (IsValid(EquippedActiveBlessingIcon) && IsValid(BlessingIcon))
	{
		BlessingIcon->SetBrushFromTexture(EquippedActiveBlessingIcon);
	}
}

void UBlessingIcon::UpdateCooldown(float RemainingTime)
{
	bool bIsCooldownValid = RemainingTime > 0.1f;
	
	if (IsValid(CooldownImage))
	{
		CooldownImage->SetVisibility(
		bIsCooldownValid ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}

	if (IsValid(CooldownTimeText))
	{
		CooldownTimeText->SetText(!bIsCooldownValid ? FText::GetEmpty() :
		(RemainingTime >= 10.f ?
		FText::FromString(FString::Printf(TEXT("%.0f"), RemainingTime)) :
		FText::FromString(FString::Printf(TEXT("%.1f"), RemainingTime)))
		);
	}
}
