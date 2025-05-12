// PayRockGames


#include "PRHwando.h"

APRHwando::APRHwando()
{
	ItemName = FString("Hwando");
	AbilityLevel = 1.f;
	ItemPrice = 300;
}

FString APRHwando::GetName() const
{
	return Super::GetName();
}

TSubclassOf<UGameplayAbility> APRHwando::GetAbilities() const
{
	return Super::GetAbilities();
}

float APRHwando::GetAbilityLevel() const
{
	return Super::GetAbilityLevel();
}

int32 APRHwando::GetItemPrice() const
{
	return Super::GetItemPrice();
}
