// PayRockGames


#include "PRGonbang.h"

APRGonbang::APRGonbang()
{
	ItemName = FString("Gonbang");
	AbilityLevel = 1.f;
	ItemPrice = 200;
}

FString APRGonbang::GetName() const
{
	return Super::GetName();
}

TSubclassOf<UGameplayAbility> APRGonbang::GetAbilities() const
{
	return Super::GetAbilities();
}

int32 APRGonbang::GetAbilityLevel() const
{
	return Super::GetAbilityLevel();
}

int32 APRGonbang::GetItemPrice() const
{
	return Super::GetItemPrice();
}
