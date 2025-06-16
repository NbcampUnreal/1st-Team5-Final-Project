#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "QuestData.generated.h"

UENUM(BlueprintType)
enum class EQuestType : uint8
{
    MonsterKill     UMETA(DisplayName = "Monster Kill"),
    ItemCollect     UMETA(DisplayName = "Item Collect")
};

USTRUCT(BlueprintType)
struct FQuestData : public FTableRowBase
{
    GENERATED_BODY()

    FQuestData()
       : QuestID(0)
       , QuestName(TEXT(""))
       , QuestDescription(TEXT(""))
       , QuestType(EQuestType::MonsterKill)
       , TargetName(TEXT(""))
       , RequiredCount(0)
       , RewardItem(TEXT(""))
       , RewardAmount(0)
    {}
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 QuestID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString QuestName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString QuestDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EQuestType QuestType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TargetName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RequiredCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RewardItem;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RewardAmount;
};