// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "SkillData.h"
#include "Engine/DataAsset.h"
#include "SkillDataAsset.generated.h"

UCLASS()
class PAYROCK_API USkillDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, FSkillData> AccessorySkills;

	UPROPERTY(EditDefaultsOnly)
	TMap<FName, FSkillData> WeaponSkills;
};
