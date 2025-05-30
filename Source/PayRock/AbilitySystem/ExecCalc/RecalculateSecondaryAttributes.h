// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RecalculateSecondaryAttributes.generated.h"


UCLASS()
class PAYROCK_API URecalculateSecondaryAttributes : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	URecalculateSecondaryAttributes();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

private:
	void AddOutputModifierForAttribute(const FGameplayAttribute& Attribute, FGameplayEffectCustomExecutionOutput& OutExecutionOutput, float NewBaseValue, float BonusValue) const;

	/*
	 *	Primary Attributes
	 */
	FGameplayEffectAttributeCaptureDefinition StrengthDef;
	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;
	FGameplayEffectAttributeCaptureDefinition DexterityDef;
	FGameplayEffectAttributeCaptureDefinition VitalityDef;
	FGameplayEffectAttributeCaptureDefinition SpiritDef;
	FGameplayEffectAttributeCaptureDefinition LuckDef;

	/*
	 *	Secondary Attributes
	 */
	FGameplayEffectAttributeCaptureDefinition ArmorDef;
	FGameplayEffectAttributeCaptureDefinition CriticalResistanceDef;
	FGameplayEffectAttributeCaptureDefinition DebuffResistanceDef;
	FGameplayEffectAttributeCaptureDefinition MaxHealthDef;
	FGameplayEffectAttributeCaptureDefinition MaxManaDef;
	FGameplayEffectAttributeCaptureDefinition HealthRegenDef;
	FGameplayEffectAttributeCaptureDefinition ManaRegenDef;
	FGameplayEffectAttributeCaptureDefinition ArmorPenetrationDef;
	FGameplayEffectAttributeCaptureDefinition CriticalChanceDef;
	FGameplayEffectAttributeCaptureDefinition CriticalDamageDef;
	FGameplayEffectAttributeCaptureDefinition MoveSpeedDef;
	FGameplayEffectAttributeCaptureDefinition AttackSpeedDef;
	FGameplayEffectAttributeCaptureDefinition CooldownReductionDef;
	FGameplayEffectAttributeCaptureDefinition LootQualityModifierDef;
	FGameplayEffectAttributeCaptureDefinition CarryWeightDef;
	FGameplayEffectAttributeCaptureDefinition BonusDamageDef;

	/*
	 *	Vital Attributes
	 */
	FGameplayEffectAttributeCaptureDefinition HealthDef;
	FGameplayEffectAttributeCaptureDefinition ManaDef;
	
};
