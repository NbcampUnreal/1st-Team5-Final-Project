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
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

private:
	void AddOutputModifierForAttribute(const FGameplayAttribute& Attribute,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput, float NewBaseValue) const;

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
	FGameplayEffectAttributeCaptureDefinition ArmorPenetrationDef;
	FGameplayEffectAttributeCaptureDefinition MaxHealthDef;
	FGameplayEffectAttributeCaptureDefinition MaxManaDef;

	/*
	 *	Vital Attributes
	 */
	FGameplayEffectAttributeCaptureDefinition HealthDef;
	FGameplayEffectAttributeCaptureDefinition ManaDef;
};
