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
	CriticalResistanceDef.AttributeToCapture = UPRAttributeSet::GetCriticalResistanceAttribute();
	DebuffResistanceDef.AttributeToCapture = UPRAttributeSet::GetDebuffResistanceAttribute();
	MaxHealthDef.AttributeToCapture = UPRAttributeSet::GetMaxHealthAttribute();
	MaxManaDef.AttributeToCapture = UPRAttributeSet::GetMaxManaAttribute();
	HealthRegenDef.AttributeToCapture = UPRAttributeSet::GetHealthRegenerationAttribute();
	ManaRegenDef.AttributeToCapture = UPRAttributeSet::GetManaRegenerationAttribute();
	ArmorPenetrationDef.AttributeToCapture = UPRAttributeSet::GetArmorPenetrationAttribute();
	CriticalChanceDef.AttributeToCapture = UPRAttributeSet::GetCriticalChanceAttribute();
	CriticalDamageDef.AttributeToCapture = UPRAttributeSet::GetCriticalDamageAttribute();
	MoveSpeedDef.AttributeToCapture = UPRAttributeSet::GetMoveSpeedAttribute();
	AttackSpeedDef.AttributeToCapture = UPRAttributeSet::GetAttackSpeedAttribute();
	CooldownReductionDef.AttributeToCapture = UPRAttributeSet::GetCooldownReductionAttribute();
	LootQualityModifierDef.AttributeToCapture = UPRAttributeSet::GetLootQualityModifierAttribute();
	CarryWeightDef.AttributeToCapture = UPRAttributeSet::GetCarryWeightAttribute();
	BonusDamageDef.AttributeToCapture = UPRAttributeSet::GetBonusDamageAttribute();

	RelevantAttributesToCapture.Add(ArmorDef);
	RelevantAttributesToCapture.Add(CriticalResistanceDef);
	RelevantAttributesToCapture.Add(DebuffResistanceDef);
	RelevantAttributesToCapture.Add(MaxHealthDef);
	RelevantAttributesToCapture.Add(MaxManaDef);
	RelevantAttributesToCapture.Add(HealthRegenDef);
	RelevantAttributesToCapture.Add(ManaRegenDef);
	RelevantAttributesToCapture.Add(ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(CriticalChanceDef);
	RelevantAttributesToCapture.Add(CriticalDamageDef);
	RelevantAttributesToCapture.Add(MoveSpeedDef);
	RelevantAttributesToCapture.Add(AttackSpeedDef);
	RelevantAttributesToCapture.Add(CooldownReductionDef);
	RelevantAttributesToCapture.Add(LootQualityModifierDef);
	RelevantAttributesToCapture.Add(CarryWeightDef);
	RelevantAttributesToCapture.Add(BonusDamageDef);

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
	 *	captured primary attribute current values
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
	 *	captured bonus secondary attribute values
	 */
	float BonusArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(ArmorDef, EvaluateParams, BonusArmor);
	float BonusCriticalResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(CriticalResistanceDef, EvaluateParams, BonusCriticalResistance);
	float BonusDebuffResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(DebuffResistanceDef, EvaluateParams, BonusDebuffResistance);
	float BonusMaxHealth = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(MaxHealthDef, EvaluateParams, BonusMaxHealth);
	float BonusMaxMana = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(MaxManaDef, EvaluateParams, BonusMaxMana);
	float BonusHealthRegeneration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(HealthRegenDef, EvaluateParams, BonusHealthRegeneration);
	float BonusManaRegeneration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(ManaRegenDef, EvaluateParams, BonusManaRegeneration);
	float BonusArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(ArmorPenetrationDef, EvaluateParams, BonusArmorPenetration);
	float BonusCriticalChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(CriticalChanceDef, EvaluateParams, BonusCriticalChance);
	float BonusCriticalDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(CriticalDamageDef, EvaluateParams, BonusCriticalDamage);
	float BonusMoveSpeed = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(MoveSpeedDef, EvaluateParams, BonusMoveSpeed);
	float BonusAttackSpeed = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(AttackSpeedDef, EvaluateParams, BonusAttackSpeed);
	float BonusCooldownReduction = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(CooldownReductionDef, EvaluateParams, BonusCooldownReduction);
	float BonusLootQualityModifier = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(LootQualityModifierDef, EvaluateParams, BonusLootQualityModifier);
	float BonusCarryWeight = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(CarryWeightDef, EvaluateParams, BonusCarryWeight);
	float AddedBonusDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(BonusDamageDef, EvaluateParams, AddedBonusDamage);

	// Captured vital attributes
	float Health = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealthDef, EvaluateParams, Health);
	float Mana = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ManaDef, EvaluateParams, Mana);
	float OldMaxHealth = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MaxHealthDef, EvaluateParams, OldMaxHealth);
	float OldMaxMana = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MaxManaDef, EvaluateParams, OldMaxMana);

	// Armor
	float NewBaseArmor = Strength * 1.5f + Vitality * 2.f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetArmorAttribute(), OutExecutionOutput, NewBaseArmor, BonusArmor);

	// Armor Penetration
	float NewBaseArmorPenetration = Strength * 0.75f + Dexterity * 0.5f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetArmorPenetrationAttribute(), OutExecutionOutput, NewBaseArmorPenetration, BonusArmorPenetration);
	
	// Critical Resistance
	float NewArmor = NewBaseArmor + BonusArmor;
	float NewBaseCriticalResistance = NewArmor * 0.1f + Spirit * 0.5f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetCriticalResistanceAttribute(), OutExecutionOutput, NewBaseCriticalResistance, BonusCriticalResistance);

	// Debuff Resistance
	float NewBaseDebuffResistance = Spirit * 1.f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetDebuffResistanceAttribute(), OutExecutionOutput, NewBaseDebuffResistance, BonusDebuffResistance);

	// Health Regeneration
	float NewBaseHealthRegeneration = Vitality * 0.2f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetHealthRegenerationAttribute(), OutExecutionOutput, NewBaseHealthRegeneration, BonusHealthRegeneration);

	// Mana Regeneration
	float NewBaseManaRegeneration = Intelligence * 0.3f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetManaRegenerationAttribute(), OutExecutionOutput, NewBaseManaRegeneration, BonusManaRegeneration);

	// Critical Chance
	float NewArmorPenetration = NewBaseArmorPenetration + BonusArmorPenetration;
	float NewBaseCriticalChance = NewArmorPenetration * 0.4f + Luck * 0.5f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetCriticalChanceAttribute(), OutExecutionOutput, NewBaseCriticalChance, BonusCriticalChance);

	// Critical Damage
	float NewBaseCriticalDamage = NewArmorPenetration * 0.2f + 150.f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetCriticalDamageAttribute(), OutExecutionOutput, NewBaseCriticalDamage, BonusCriticalDamage);

	// Move Speed
	float NewBaseMoveSpeed = Dexterity * 0.3f + 100.f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetMoveSpeedAttribute(), OutExecutionOutput, NewBaseMoveSpeed, BonusMoveSpeed);

	// Attack Speed
	float NewBaseAttackSpeed = Dexterity * 0.5f + 100.f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetAttackSpeedAttribute(), OutExecutionOutput, NewBaseAttackSpeed, BonusAttackSpeed);

	// Cooldown Reduction
	float NewBaseCooldownReduction = Spirit * 0.4f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetCooldownReductionAttribute(), OutExecutionOutput, NewBaseCooldownReduction, BonusCooldownReduction);

	// Loot Quality Modifier
	float NewBaseLootQualityModifier = Luck * 1.f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetLootQualityModifierAttribute(), OutExecutionOutput, NewBaseLootQualityModifier, BonusLootQualityModifier);

	// Carry Weight
	float NewBaseCarryWeight = Strength * 5.f + 100.f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetCarryWeightAttribute(), OutExecutionOutput, NewBaseCarryWeight, BonusCarryWeight);

	// Bonus Damage
	AddOutputModifierForAttribute(UPRAttributeSet::GetBonusDamageAttribute(), OutExecutionOutput, 0, AddedBonusDamage);

	// MaxHealth
	float NewBaseMaxHealth = Vitality * 10.f + Strength * 5.f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetMaxHealthAttribute(), OutExecutionOutput, NewBaseMaxHealth, BonusMaxHealth);

	// MaxMana
	float NewBaseMaxMana = Vitality * 10.f + Strength * 5.f;
	AddOutputModifierForAttribute(UPRAttributeSet::GetMaxManaAttribute(), OutExecutionOutput, NewBaseMaxMana, BonusMaxMana);

	/*
	 *	MaxHealth / MaxMana change requires Health / Mana ratio to be maintained
	 */
	
	// Apply pre-change Health Ratio
	float HealthRatio = OldMaxHealth > 0.f ? Health / OldMaxHealth : 1.f;
	float NewMaxHealth = NewBaseMaxHealth + BonusMaxHealth;
	float NewHealth = FMath::Clamp(NewMaxHealth * HealthRatio, 0.f, NewMaxHealth);
	AddOutputModifierForAttribute(UPRAttributeSet::GetHealthAttribute(), OutExecutionOutput, NewHealth, 0);
	
	// Apply pre-change Mana Ratio
	float ManaRatio = OldMaxMana > 0.f ? Mana / OldMaxMana : 1.f;
	float NewMaxMana = NewBaseMaxMana + BonusMaxMana;
	float NewMana = FMath::Clamp(NewMaxMana * ManaRatio, 0.f, NewMaxMana);
	AddOutputModifierForAttribute(UPRAttributeSet::GetManaAttribute(), OutExecutionOutput, NewMana, 0);
}

void URecalculateSecondaryAttributes::AddOutputModifierForAttribute(
	const FGameplayAttribute& Attribute, FGameplayEffectCustomExecutionOutput& OutExecutionOutput,
	float NewBaseValue, float BonusValue) const
{
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		Attribute,
		EGameplayModOp::Override,
		NewBaseValue
	));

	if (FMath::IsNearlyZero(BonusValue)) return;
	
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		Attribute,
		EGameplayModOp::AddFinal,
		BonusValue
	));
}

