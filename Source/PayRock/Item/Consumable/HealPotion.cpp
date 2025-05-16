#include "PayRock/Item/Consumable/HealPotion.h"
#include "HealPotion.h"

AHealPotion::AHealPotion()
{
    ItemName = FString("HealPotion");
    AbilityLevel = 1.f;
    ItemPrice = 100;
}


FString AHealPotion::GetName() const
{
    return Super::GetName();
}

TSubclassOf<UGameplayAbility> AHealPotion::GetAbilities() const
{
    return Super::GetAbilities(); //ga����
}

float AHealPotion::GetAbilityLevel() const
{
    return Super::GetAbilityLevel();
}

int32 AHealPotion::GetItemPrice() const
{
    return Super::GetItemPrice();
}