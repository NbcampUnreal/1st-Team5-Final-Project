// PayRockGames


#include "SkillInfoWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "PayRock/UI/WidgetController/StatWidgetController.h"

void USkillInfoWidget::OnWidgetControllerSet()
{
	UStatWidgetController* StatWC = Cast<UStatWidgetController>(WidgetController);
	if (!IsValid(StatWC) || !IsValid(SkillType)) return;
	
	switch (Type)
	{
	case ESkillType::Blessing:
		StatWC->OnActiveBlessingInfoSet.AddUniqueDynamic(this, &USkillInfoWidget::SetBlessingSkillInfo);
		SkillType->SetText(FText::FromString(TEXT("위인의 넋")));
		break;
	case ESkillType::Accessory:
		StatWC->OnAccessorySkillChanged.AddUniqueDynamic(this, &USkillInfoWidget::SetItemSkillInfo);
		SkillType->SetText(FText::FromString(TEXT("장신구 능력")));
		break;
	case ESkillType::Weapon:
		StatWC->OnWeaponSkillChanged.AddUniqueDynamic(this, &USkillInfoWidget::SetItemSkillInfo);
		SkillType->SetText(FText::FromString(TEXT("무기 능력")));
		break;
	default: ;
	}
}

void USkillInfoWidget::SetBlessingSkillInfo(const FBlessingData& BlessingData)
{
	if (BlessingData.KoreanName.IsEmpty())
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
	if (IsValid(SkillName))
	{
		SkillName->SetText(BlessingData.KoreanName);
	}
	if (IsValid(SkillDescription))
	{
		SkillDescription->SetText(BlessingData.BlessingDescription);
	}
	if (IsValid(PenaltyDescription))
	{
		PenaltyDescription->SetText(BlessingData.PenaltyDescription);
	}
	if (IsValid(SkillImage))
	{
		SkillImage->SetBrushFromTexture(BlessingData.Icon);
	}
}

void USkillInfoWidget::SetItemSkillInfo(const FSkillData& SkillData)
{
	if (SkillData.DisplayName.IsEmpty())
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	if (IsValid(SkillName))
	{
		SkillName->SetText(SkillData.DisplayName);
	}
	if (IsValid(SkillDescription))
	{
		SkillDescription->SetText(SkillData.Description);
	}
	if (IsValid(SkillImage))
	{
		SkillImage->SetBrushFromTexture(SkillData.SkillIcon);
	}
	if (IsValid(ManaCost))
	{
		ManaCost->SetText(SkillData.ManaCost);
	}
	if (IsValid(Cooldown))
	{
		ManaCost->SetText(SkillData.Cooldown);
	}
}


