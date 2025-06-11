// PayRockGames


#include "MMC_BlessingCooldown.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"

UMMC_BlessingCooldown::UMMC_BlessingCooldown()
{
	CooldownReductionDef.AttributeToCapture = UPRAttributeSet::GetCooldownReductionAttribute();
	CooldownReductionDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	CooldownReductionDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(CooldownReductionDef);
}

float UMMC_BlessingCooldown::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTags;
	EvaluateParams.TargetTags = TargetTags;

	float CooldownReduction = 0.f;
	GetCapturedAttributeMagnitude(CooldownReductionDef, Spec, EvaluateParams, CooldownReduction);

	return BaseMagnitude * (1 - CooldownReduction * 0.01);
}


