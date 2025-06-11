// PayRockGames


#include "RecalculateSecondaryAttributes.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"

URecalculateSecondaryAttributes::URecalculateSecondaryAttributes()
{
	/*
	 *	Primary Attributes
	 */
	StrengthDef.AttributeToCapture = UPRAttributeSet::GetStrengthAttribute();
	IntelligenceDef.AttributeToCapture = UPRAttributeSet::GetIntelligenceAttribute();
	DexterityDef.AttributeToCapture = UPRAttributeSet::GetDexterityAttribute();
	VitalityDef.AttributeToCapture = UPRAttributeSet::GetVitalityAttribute();
	SpiritDef.AttributeToCapture = UPRAttributeSet::GetSpiritAttribute();
	LuckDef.AttributeToCapture = UPRAttributeSet::GetLuckAttribute();
	
	RelevantAttributesToCapture.Add(StrengthDef);
	RelevantAttributesToCapture.Add(IntelligenceDef);
	RelevantAttributesToCapture.Add(DexterityDef);
	RelevantAttributesToCapture.Add(VitalityDef);
	RelevantAttributesToCapture.Add(SpiritDef);
	RelevantAttributesToCapture.Add(LuckDef);

	/*
	 *	Secondary Attributes
	 */
	ArmorDef.AttributeToCapture = UPRAttributeSet::GetArmorAttribute();
	ArmorPenetrationDef.AttributeToCapture = UPRAttributeSet::GetArmorPenetrationAttribute();
	MaxHealthDef.AttributeToCapture = UPRAttributeSet::GetMaxHealthAttribute();
	MaxManaDef.AttributeToCapture = UPRAttributeSet::GetMaxManaAttribute();

	RelevantAttributesToCapture.Add(ArmorDef);
	RelevantAttributesToCapture.Add(ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(MaxHealthDef);
	RelevantAttributesToCapture.Add(MaxManaDef);

	/*
	 *	Vital Attributes
	 */
	HealthDef.AttributeToCapture = UPRAttributeSet::GetHealthAttribute();
	ManaDef.AttributeToCapture = UPRAttributeSet::GetManaAttribute();

	RelevantAttributesToCapture.Add(HealthDef);
	RelevantAttributesToCapture.Add(ManaDef);
}

void URecalculateSecondaryAttributes::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	const FGameplayEffectSpec& SpecRef = ExecutionParams.GetOwningSpec();
	UAbilitySystemComponent* ASC = ExecutionParams.GetTargetAbilitySystemComponent();
	if (!ASC) return;

	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SpecRef.CapturedSourceTags.GetAggregatedTags();
	EvaluateParams.TargetTags = SpecRef.CapturedTargetTags.GetAggregatedTags();

	/*
	 *	captured primary attribute "current" values
	 */
	float Strength = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(StrengthDef, EvaluateParams, Strength);
	float Intelligence = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(IntelligenceDef, EvaluateParams, Intelligence);
	float Dexterity = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DexterityDef, EvaluateParams, Dexterity);
	float Vitality = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(VitalityDef, EvaluateParams, Vitality);
	float Spirit = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(SpiritDef, EvaluateParams, Spirit);
	float Luck = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(LuckDef, EvaluateParams, Luck);
	
	/*
	 *	captured secondary attribute "bonus" values
	 */
	float BonusArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(ArmorDef, EvaluateParams, BonusArmor);
	float BonusArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(ArmorPenetrationDef, EvaluateParams, BonusArmorPenetration);
	float BonusMaxHealth = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(MaxHealthDef, EvaluateParams, BonusMaxHealth);
	float BonusMaxMana = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(MaxManaDef, EvaluateParams, BonusMaxMana);

	// Captured vital attributes
	float Health = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealthDef, EvaluateParams, Health);
	float Mana = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ManaDef, EvaluateParams, Mana);
	float OldMaxHealth = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeBaseValue(MaxHealthDef, OldMaxHealth);
	float OldMaxMana = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeBaseValue(MaxManaDef, OldMaxMana);

	/*
	 *	Luck-based primary stat bonus
	 */
	float LuckBonus = 0.2f * Luck;
	Strength += LuckBonus;
	Intelligence += LuckBonus;
	Dexterity += LuckBonus;
	Vitality += LuckBonus;
	Spirit += LuckBonus;
	AddOutputModifierForAttribute(UPRAttributeSet::GetStrengthAttribute(), OutExecutionOutput, Strength);
	AddOutputModifierForAttribute(UPRAttributeSet::GetIntelligenceAttribute(), OutExecutionOutput, Intelligence);
	AddOutputModifierForAttribute(UPRAttributeSet::GetDexterityAttribute(), OutExecutionOutput, Dexterity);
	AddOutputModifierForAttribute(UPRAttributeSet::GetVitalityAttribute(), OutExecutionOutput, Vitality);
	AddOutputModifierForAttribute(UPRAttributeSet::GetSpiritAttribute(), OutExecutionOutput, Spirit);
	
	/*
	 *	Recalculating Base Values (secondary)
	 */
	// Armor
	float NewBaseArmor = Strength * 1.5f + Vitality * 2.f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetArmorAttribute(), OutExecutionOutput, NewBaseArmor/*, BonusArmor*/);

	// Armor Penetration
	float NewBaseArmorPenetration = Strength * 0.75f + Dexterity * 0.5f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetArmorPenetrationAttribute(), OutExecutionOutput, NewBaseArmorPenetration/*, BonusArmorPenetration*/);
	
	// Critical Resistance
	float NewArmor = NewBaseArmor + BonusArmor;
	float NewBaseCriticalResistance = NewArmor * 0.1f + Spirit * 0.5f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetCriticalResistanceAttribute(), OutExecutionOutput, NewBaseCriticalResistance/*, BonusCriticalResistance*/);

	// Debuff Resistance
	float NewBaseDebuffResistance = Spirit * 1.f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetDebuffResistanceAttribute(), OutExecutionOutput, NewBaseDebuffResistance/*, BonusDebuffResistance*/);

	// Health Regeneration
	float NewBaseHealthRegeneration = Vitality * 0.2f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetHealthRegenerationAttribute(), OutExecutionOutput, NewBaseHealthRegeneration/*, BonusHealthRegeneration*/);

	// Mana Regeneration
	float NewBaseManaRegeneration = Intelligence * 0.3f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetManaRegenerationAttribute(), OutExecutionOutput, NewBaseManaRegeneration/*, BonusManaRegeneration*/);

	// Critical Chance
	float NewArmorPenetration = NewBaseArmorPenetration + BonusArmorPenetration;
	float NewBaseCriticalChance = NewArmorPenetration * 0.4f + Luck * 0.5f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetCriticalChanceAttribute(), OutExecutionOutput, NewBaseCriticalChance/*, BonusCriticalChance*/);

	// Critical Damage
	float NewBaseCriticalDamage = NewArmorPenetration * 0.2f + 150.f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetCriticalDamageAttribute(), OutExecutionOutput, NewBaseCriticalDamage/*, BonusCriticalDamage*/);

	// Move Speed
	float NewBaseMoveSpeed = Dexterity * 0.3f + 100.f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetMoveSpeedAttribute(), OutExecutionOutput, NewBaseMoveSpeed/*, BonusMoveSpeed*/);

	// Attack Speed
	float NewBaseAttackSpeed = Dexterity * 0.5f + 100.f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetAttackSpeedAttribute(), OutExecutionOutput, NewBaseAttackSpeed/*, BonusAttackSpeed*/);

	// Cooldown Reduction
	float NewBaseCooldownReduction = Spirit * 0.4f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetCooldownReductionAttribute(), OutExecutionOutput, NewBaseCooldownReduction/*, BonusCooldownReduction*/);

	// Loot Quality Modifier
	float NewBaseLootQualityModifier = Luck * 1.f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetLootQualityModifierAttribute(), OutExecutionOutput, NewBaseLootQualityModifier/*, BonusLootQualityModifier*/);

	// Carry Weight
	float NewBaseCarryWeight = Strength * 5.f + 100.f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetCarryWeightAttribute(), OutExecutionOutput, NewBaseCarryWeight);

	// Bonus Damage
 	AddOutputModifierForAttribute(UPRAttributeSet::GetBonusDamageAttribute(), OutExecutionOutput, 0);

	// MaxHealth
	float NewBaseMaxHealth = Vitality * 10.f + Strength * 5.f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetMaxHealthAttribute(), OutExecutionOutput, NewBaseMaxHealth);

	// MaxMana
	float NewBaseMaxMana = Intelligence * 15.f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetMaxManaAttribute(), OutExecutionOutput, NewBaseMaxMana);

	/*
	 *	MaxHealth / MaxMana change requires Health / Mana ratio to be maintained
	 */
	
	// Apply pre-change Health Ratio
	float HealthRatio = OldMaxHealth > 0.f ? Health / OldMaxHealth : 1.f;
	float NewMaxHealth = NewBaseMaxHealth + BonusMaxHealth;
	float NewHealth = FMath::Clamp(NewMaxHealth * HealthRatio, 0.f, NewMaxHealth);
	AddOutputModifierForAttribute(UPRAttributeSet::GetHealthAttribute(), OutExecutionOutput, NewHealth/*, 0*/);
	
	// Apply pre-change Mana Ratio
	float ManaRatio = OldMaxMana > 0.f ? Mana / OldMaxMana : 1.f;
	float NewMaxMana = NewBaseMaxMana + BonusMaxMana;
	float NewMana = FMath::Clamp(NewMaxMana * ManaRatio, 0.f, NewMaxMana);
	AddOutputModifierForAttribute(UPRAttributeSet::GetManaAttribute(), OutExecutionOutput, NewMana/*, 0*/);
}

void URecalculateSecondaryAttributes::AddOutputModifierForAttribute(
	const FGameplayAttribute& Attribute, FGameplayEffectCustomExecutionOutput& OutExecutionOutput,
	float NewBaseValue) const
{
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		Attribute,
		EGameplayModOp::Override,
		NewBaseValue
	));
}
