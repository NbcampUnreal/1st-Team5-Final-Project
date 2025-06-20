// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "SkillData.generated.h"

UENUM(BlueprintType)
enum class ESkillItem : uint8
{
	Accessory,
	Weapon,
	MAX
};

USTRUCT(BlueprintType)
struct FSkillData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText InputKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> SkillIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ManaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cooldown;

	FSkillData()
		: ItemID(TEXT("")), DisplayName(FText()), Description(FText()), InputKey(FText()), SkillIcon(nullptr),
	ManaCost(0.0f), Cooldown(0.0f) 
	{}
};