// PayRockGames


#include "PRSSangeom.h"

APRSSangeom::APRSSangeom()
{
	ItemName = FString("SSangeom");
	AbilityLevel = 1.f;
	ItemPrice = 300;
}

FString APRSSangeom::GetName() const
{
	return Super::GetName();
}

TSubclassOf<UGameplayAbility> APRSSangeom::GetAbilities() const
{
	return Super::GetAbilities();
}

int32 APRSSangeom::GetAbilityLevel() const
{
	return Super::GetAbilityLevel();
}

int32 APRSSangeom::GetItemPrice() const
{
	return Super::GetItemPrice();
}
