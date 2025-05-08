// PayRockGames

#include "MMC_MaxHealth.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"
#include "PayRock/Character/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VitalityDef.AttributeToCapture = UPRAttributeSet::GetVitalityAttribute();
	VitalityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VitalityDef.bSnapshot = false;
	
	StrengthDef.AttributeToCapture = UPRAttributeSet::GetStrengthAttribute();
	StrengthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	StrengthDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VitalityDef);
	RelevantAttributesToCapture.Add(StrengthDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTags;
	EvaluateParams.TargetTags = TargetTags;

	float Vitality = 0.f;
	float Strength = 0.f;
	GetCapturedAttributeMagnitude(VitalityDef, Spec, EvaluateParams, Vitality);
	GetCapturedAttributeMagnitude(StrengthDef, Spec, EvaluateParams, Strength);
	Vitality = FMath::Max(Vitality, 0.f);
	Strength = FMath::Max(Strength, 0.f);

	int32 Level = 0;
	if (TScriptInterface<ICombatInterface> CombatInterface = Spec.GetContext().GetSourceObject())
	{
		Level = CombatInterface->GetCharacterLevel();
	}

	return Vitality * 10 + Strength * 5 + Level * 10;
}
