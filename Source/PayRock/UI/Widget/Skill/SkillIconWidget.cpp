// PayRockGames


#include "SkillIconWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "PayRock/PRGameplayTags.h"
#include "PayRock/UI/WidgetController/OverlayWidgetController.h"

void USkillIconWidget::OnWidgetControllerSet()
{
	if (UOverlayWidgetController* OverlayWC = Cast<UOverlayWidgetController>(WidgetController))
	{
		switch (SkillItem)
		{
		case ESkillItem::Accessory:
			OverlayWC->OnAccessorySkillChanged.AddUniqueDynamic(this, &USkillIconWidget::OnSkillChanged);
			break;
		case ESkillItem::Weapon:
			OverlayWC->OnWeaponSkillChanged.AddUniqueDynamic(this, &USkillIconWidget::OnSkillChanged);
			break;
		default:
		}
		
		FOnCooldownChanged& Delegate =
			OverlayWC->CooldownDelegates.FindOrAdd(FPRGameplayTags::Get().Ability_AccessorySkill_Cooldown);
		Delegate.AddUniqueDynamic(this, &USkillIconWidget::UpdateCooldown);
	}
}

void USkillIconWidget::OnSkillChanged(const FSkillData& SkillData)
{
	if (IsValid(SkillIcon) && IsValid(SkillData.SkillIcon))
	{
		SkillIcon->SetBrushFromTexture(SkillData.SkillIcon);
	}
}

void USkillIconWidget::UpdateCooldown(float RemainingTime)
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
