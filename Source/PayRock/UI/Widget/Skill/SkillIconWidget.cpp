// PayRockGames


#include "SkillIconWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "PayRock/PRGameplayTags.h"
#include "PayRock/UI/WidgetController/NecroWidgetController.h"
#include "PayRock/UI/WidgetController/OverlayWidgetController.h"

void USkillIconWidget::OnWidgetControllerSet()
{
	if (UOverlayWidgetController* OverlayWC = Cast<UOverlayWidgetController>(WidgetController))
	{
		FOnCooldownChanged& AccessoryCooldownDelegate =
			OverlayWC->CooldownDelegates.FindOrAdd(FPRGameplayTags::Get().Ability_AccessorySkill_Cooldown);
		FOnCooldownChanged& WeaponCooldownDelegate =
			OverlayWC->CooldownDelegates.FindOrAdd(FPRGameplayTags::Get().Ability_WeaponSkill_Cooldown);
		FOnCooldownChanged& BlessingCooldownDelegate =
			OverlayWC->CooldownDelegates.FindOrAdd(FPRGameplayTags::Get().Ability_Blessing_Cooldown);
			
		if (CooldownTag.MatchesTagExact(FPRGameplayTags::Get().Ability_AccessorySkill_Cooldown))
		{
			OverlayWC->OnAccessorySkillChanged.AddUniqueDynamic(this, &USkillIconWidget::OnSkillChanged);
			AccessoryCooldownDelegate.AddUniqueDynamic(this, &USkillIconWidget::UpdateCooldown);
		}
		else if (CooldownTag.MatchesTagExact(FPRGameplayTags::Get().Ability_WeaponSkill_Cooldown))
		{
			OverlayWC->OnWeaponSkillChanged.AddUniqueDynamic(this, &USkillIconWidget::OnSkillChanged);
			WeaponCooldownDelegate.AddUniqueDynamic(this, &USkillIconWidget::UpdateCooldown);
		}
		else if (CooldownTag.MatchesTagExact(FPRGameplayTags::Get().Ability_Blessing_Cooldown))
		{
			OverlayWC->OnActiveBlessingChanged.AddUniqueDynamic(this, &USkillIconWidget::OnSkillChanged);
			BlessingCooldownDelegate.AddUniqueDynamic(this, &USkillIconWidget::UpdateCooldown);
		}
	}
	else if (UNecroWidgetController* NecroWC = Cast<UNecroWidgetController>(WidgetController))
	{
		FOnCooldownChanged& NecroExpelCooldownDelegate =
			NecroWC->CooldownDelegates.FindOrAdd(FPRGameplayTags::Get().Ability_Necro_Cooldown_Expel);
		FOnCooldownChanged& NecroSlowCooldownDelegate =
			NecroWC->CooldownDelegates.FindOrAdd(FPRGameplayTags::Get().Ability_Necro_Cooldown_Slow);
		FOnCooldownChanged& NecroBlindCooldownDelegate =
			NecroWC->CooldownDelegates.FindOrAdd(FPRGameplayTags::Get().Ability_Necro_Cooldown_Blind);

		if (CooldownTag.MatchesTagExact(FPRGameplayTags::Get().Ability_Necro_Cooldown_Expel))
		{
			NecroExpelCooldownDelegate.AddUniqueDynamic(this, &USkillIconWidget::UpdateCooldown);
		}
		else if (CooldownTag.MatchesTagExact(FPRGameplayTags::Get().Ability_Necro_Cooldown_Slow))
		{
			NecroSlowCooldownDelegate.AddUniqueDynamic(this, &USkillIconWidget::UpdateCooldown);
		}
		else if (CooldownTag.MatchesTagExact(FPRGameplayTags::Get().Ability_Necro_Cooldown_Blind))
		{
			NecroBlindCooldownDelegate.AddUniqueDynamic(this, &USkillIconWidget::UpdateCooldown);
		}
	}
}

void USkillIconWidget::OnSkillChanged(UTexture2D* IconTexture)
{
	if (IsValid(SkillIcon))
	{
		if (IsValid(IconTexture))
		{
			SkillIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SkillIcon->SetBrushFromTexture(IconTexture);
		}
		else
		{
			SkillIcon->SetVisibility(ESlateVisibility::Collapsed);
		}
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
