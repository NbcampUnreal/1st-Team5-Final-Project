// PayRockGames


#include "PRHyeobdo.h"

APRHyeobdo::APRHyeobdo()
{
	ItemName = FString("Hyeobdo");
	AbilityLevel = 1.f;
	ItemPrice = 350;
}

FString APRHyeobdo::GetName() const
{
	return Super::GetName();
}

TSubclassOf<UGameplayAbility> APRHyeobdo::GetAbilities() const
{
	return Super::GetAbilities();
}

int32 APRHyeobdo::GetAbilityLevel() const
{
	return Super::GetAbilityLevel();
}

int32 APRHyeobdo::GetItemPrice() const
{
	return Super::GetItemPrice();
}
