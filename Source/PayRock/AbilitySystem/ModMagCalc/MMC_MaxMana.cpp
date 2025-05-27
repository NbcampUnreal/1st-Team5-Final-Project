// PayRockGames

#include "MMC_MaxMana.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"
#include "PayRock/Character/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDef.AttributeToCapture = UPRAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTags;
	EvaluateParams.TargetTags = TargetTags;

	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluateParams, Intelligence);
	Intelligence = FMath::Max(Intelligence, 0.f);

	int32 Level = 0;
	if (TScriptInterface<ICombatInterface> CombatInterface = Spec.GetContext().GetSourceObject())
	{
		Level = CombatInterface->GetCharacterLevel();
	}
	return Intelligence * 15 + Level * 10;
}
