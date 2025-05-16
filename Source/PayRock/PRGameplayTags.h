// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * PRGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */
struct FPRGameplayTags
{
public:
	static const FPRGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();
	
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Dexterity;
	FGameplayTag Attributes_Primary_Vitality;
	FGameplayTag Attributes_Primary_Spirit;
	FGameplayTag Attributes_Primary_Luck;
	
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalResistance;
	FGameplayTag Attributes_Secondary_DebuffResistance;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_CriticalChance;
	FGameplayTag Attributes_Secondary_CriticalDamage;
	FGameplayTag Attributes_Secondary_MoveSpeed;
	FGameplayTag Attributes_Secondary_AttackSpeed;
	FGameplayTag Attributes_Secondary_CooldownReduction;
	FGameplayTag Attributes_Secondary_LootQualityModifier;
	FGameplayTag Attributes_Secondary_CarryWeight;

	FGameplayTag Attributes_Vital_Health;
	FGameplayTag Attributes_Vital_Mana;

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;

	FGameplayTag Player_Block_InputPressed;
	FGameplayTag Player_Block_InputHeld;
	FGameplayTag Player_Block_InputReleased;

	FGameplayTag Damage_Physical;
	FGameplayTag Damage_Magic;

	FGameplayTag Status_Life_Dead;
	FGameplayTag Status_Life_Alive;

	FGameplayTag Effects_HitReact;

	FGameplayTag Ability_Attack;
	FGameplayTag Cooldown_Attack;
private:
	static FPRGameplayTags GameplayTags;
};
