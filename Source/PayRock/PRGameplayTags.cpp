// PayRockGames

#include "PRGameplayTags.h"
#include "GameplayTagsManager.h"

// This is necessary in order to make the variable static
FPRGameplayTags FPRGameplayTags::GameplayTags;

void FPRGameplayTags::InitializeNativeGameplayTags()
{
    /*
     * Primary Attributes
     */
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength")
		);
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Primary.Strength")
        );
    GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Primary.Intelligence")
        );
    GameplayTags.Attributes_Primary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Primary.Dexterity")
        );
    GameplayTags.Attributes_Primary_Vitality = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Primary.Vitality")
        );
    GameplayTags.Attributes_Primary_Spirit = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Primary.Spirit")
        );
    GameplayTags.Attributes_Primary_Luck = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Primary.Luck")
        );

    /*
     * Secondary Attributes
     */
    GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.Armor")
        );
    GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.BlockChance")
        );
    GameplayTags.Attributes_Secondary_CriticalResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.CriticalResistance")
        );
    GameplayTags.Attributes_Secondary_DebuffResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.DebuffResistance")
        );
    GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.MaxHealth")
        );
    GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.MaxMana")
        );
    GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.HealthRegeneration")
        );
    GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.ManaRegeneration")
        );
    GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.ArmorPenetration")
        );
    GameplayTags.Attributes_Secondary_CriticalChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.CriticalChance")
        );
    GameplayTags.Attributes_Secondary_CriticalDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.CriticalDamage")
        );
    GameplayTags.Attributes_Secondary_MoveSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.MoveSpeed")
        );
    GameplayTags.Attributes_Secondary_AttackSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.AttackSpeed")
        );
    GameplayTags.Attributes_Secondary_CooldownReduction = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.CooldownReduction")
        );
    GameplayTags.Attributes_Secondary_LootQualityModifier = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.LootQualityModifier")
        );
    GameplayTags.Attributes_Secondary_CarryWeight = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.CarryWeight")
        );

    /*
     * Vital Attributes
     */
    GameplayTags.Attributes_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Vital.Health")
        );
    GameplayTags.Attributes_Vital_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Vital.Mana")
        );

    /*
     * Input
     */
    GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("InputTag.LMB")
        );
    GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("InputTag.1")
        );
    GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("InputTag.2")
        );
    GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("InputTag.3")
        );
    GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("InputTag.4")
        );
    GameplayTags.InputTag_Z = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("InputTag.Z")
        );

    /*
     * Combat Socket
     */
    GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
       FName("CombatSocket.Weapon")
       );
    GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("CombatSocket.RightHand")
        );
    GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("CombatSocket.LeftHand")
        );

    /*
     * Block Input
     */
    GameplayTags.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(
       FName("Player.Block.InputHeld")
       );
    GameplayTags.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(
       FName("Player.Block.InputPressed")
       );
    GameplayTags.Player_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(
       FName("Player.Block.InputReleased")
       );

    /*
     * Damage
     */
    GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
       FName("Damage.Physical")
       );
    GameplayTags.Damage_Magic = UGameplayTagsManager::Get().AddNativeGameplayTag(
       FName("Damage.Magic")
       );

    /*
    *   Character Status
    */
    GameplayTags.Status_Life_Alive = UGameplayTagsManager::Get().AddNativeGameplayTag(
       FName("Status.Life.Alive")
       );
    GameplayTags.Status_Life_Dead = UGameplayTagsManager::Get().AddNativeGameplayTag(
       FName("Status.Life.Dead")
       );
    GameplayTags.Status_Weapon_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
       FName("Status.Weapon.Equipped")
       );
    GameplayTags.Status_Weapon_Unarmed = UGameplayTagsManager::Get().AddNativeGameplayTag(
       FName("Status.Weapon.Unarmed")
       );

    /*
     *  Buff
     */
    GameplayTags.Status_Buff_Invincible = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Status.Buff.Invincible")
        );
    GameplayTags.Status_Buff_Invisible = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Status.Buff.Invisible")
        ); 
    GameplayTags.Status_Buff_BackAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Status.Buff.BackAttack")
        );
    GameplayTags.Status_Buff_DoubleJump = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Status.Buff.DoubleJump")
        );
    GameplayTags.Status_Buff_LifeSteal = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Status.Buff.LifeSteal")
        );

    /*
     *  Debuff
     */
    GameplayTags.Status_Debuff_Poison = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Status.Debuff.Poison")
        );
    GameplayTags.Status_Debuff_Blind = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Status.Debuff.Blind")
        );

    /*
     *  Necro
     */
    GameplayTags.Status_Necro_Visible = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Status.Necro.Visible")
        );

    /*
     *  Cooldown
     */
    GameplayTags.Ability_Blessing_Cooldown = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Blessing.Cooldown")
        );
    
    /*
     * Effects
     */
    GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
       FName("Effects.HitReact")
       );

    GameplayTags.Ability_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Attack")
        );
    GameplayTags.Cooldown_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Cooldown.Attack")
        );
}
