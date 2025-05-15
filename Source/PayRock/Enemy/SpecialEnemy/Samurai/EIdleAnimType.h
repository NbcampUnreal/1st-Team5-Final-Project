// PayRockGames

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EIdleAnimType : uint8
{
	Ninja	UMETA(DisplayName = "Ninja"),
	Mutant	UMETA(DisplayName = "Mutant"),
	Pistol	UMETA(DisplayName = "Pistol"),
	SwordAndShield UMETA(DisplayName = "SwordAndShield")
};