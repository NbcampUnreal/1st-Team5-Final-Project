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
		if (FOnCooldownChanged* DelegatePtr =
			OverlayWC->CooldownDelegates.Find(FPRGameplayTags::Get().Ability_Blessing_Cooldown))
		{
			DelegatePtr->AddUniqueDynamic(this, &UBlessingIcon::UpdateCooldown);
		}
	}
}

void UBlessingIcon::OnActiveBlessingChanged(const FBlessingData& Blessing)
{
	if (IsValid(Blessing.Icon) && IsValid(BlessingIcon))
	{
		BlessingIcon->SetBrushFromTexture(Blessing.Icon);
	}
}

void UBlessingIcon::UpdateCooldown(float RemainingTime)
{
	bool bIsCooldownValid = RemainingTime > KINDA_SMALL_NUMBER;
	
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
