// PayRockGames


#include "PRDuengpa.h"

APRDuengpa::APRDuengpa()
{
	ItemName = FString("Duengpa");
	AbilityLevel = 1.f;
	ItemPrice = 150;
}

FString APRDuengpa::GetName() const
{
	return Super::GetName();
}

TSubclassOf<UGameplayAbility> APRDuengpa::GetAbilities() const
{
	return Super::GetAbilities();
}

int32 APRDuengpa::GetAbilityLevel() const
{
	return Super::GetAbilityLevel();
}

int32 APRDuengpa::GetItemPrice() const
{
	return Super::GetItemPrice();
}
