// PayRockGames


#include "PRWoldo.h"

APRWoldo::APRWoldo()
{
	ItemName = FString("Woldo");
	AbilityLevel = 1.f;
	ItemPrice = 400;
}

FString APRWoldo::GetName() const
{
	return Super::GetName();
}

TSubclassOf<UGameplayAbility> APRWoldo::GetAbilities() const
{
	return Super::GetAbilities();
}

int32 APRWoldo::GetAbilityLevel() const
{
	return Super::GetAbilityLevel();
}

int32 APRWoldo::GetItemPrice() const
{
	return Super::GetItemPrice();
}
