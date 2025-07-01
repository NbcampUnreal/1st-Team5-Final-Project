// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "SkillData.generated.h"

USTRUCT(BlueprintType)
struct FSkillData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> SkillIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ManaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Cooldown;

	FSkillData()
		: DisplayName(FText()), Description(FText()), SkillIcon(nullptr), ManaCost(FText()), Cooldown(FText())
	{}
};