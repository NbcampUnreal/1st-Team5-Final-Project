// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "QuestSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UQuestSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Quest")
	int32 CurrentQuestID;

	UPROPERTY(VisibleAnywhere, Category = "Quest")
	int32 CurrentProgressCount;
};
