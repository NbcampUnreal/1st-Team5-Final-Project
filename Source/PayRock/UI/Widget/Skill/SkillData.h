// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "SkillData.generated.h"

USTRUCT(BlueprintType)
struct FSkillData
{
	GENERATED_BODY()

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText InputKey;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> SkillIcon;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ManaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cooldown;*/

	FSkillData()
		: SkillIcon(nullptr) /*, ItemID(TEXT("")), DisplayName(FText()), Description(FText()), InputKey(FText()), 
	ManaCost(0.0f), Cooldown(0.0f)*/ 
	{}
};