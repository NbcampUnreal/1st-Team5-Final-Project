// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "PRItemEnum.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Unarmed     UMETA(DisplayName = "Unarmed"),
	Hyeobdo      UMETA(DisplayName = "Hyeobdo"),
	Hwando       UMETA(DisplayName = "Hwando"),
	Gonbang     UMETA(DisplayName = "Gonbang"),
	SsanggeomL    UMETA(DisplayName = "SsanggeomL"),
	SsanggeomR    UMETA(DisplayName = "SsanggeomR"),
	Ssangsudo    UMETA(DisplayName = "Ssangsudo"),
	Woldo    UMETA(DisplayName = "Woldo"),
	Duengpae UMETA(DisplayName = "Duengpae")
};

UCLASS()
class PAYROCK_API UPRItemEnum : public UUserDefinedEnum
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EWeaponType MyWeapon;
};
