// PayRockGames


#include "PRSSansudo.h"

APRSSansudo::APRSSansudo()
{
	ItemName = FString("SSansudo");
	AbilityLevel = 1.f;
	ItemPrice = 500;
}

FString APRSSansudo::GetName() const
{
	return Super::GetName();
}

TSubclassOf<UGameplayAbility> APRSSansudo::GetAbilities() const
{
	return Super::GetAbilities();
}

int32 APRSSansudo::GetAbilityLevel() const
{
	return Super::GetAbilityLevel();
}

int32 APRSSansudo::GetItemPrice() const
{
	return Super::GetItemPrice();
}
