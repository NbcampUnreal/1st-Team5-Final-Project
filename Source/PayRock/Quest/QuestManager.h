// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "QuestData.h"
#include "QuestSaveGame.h"
#include "QuestManager.generated.h"

/**
 * 
 */

UCLASS(Blueprintable)
class PAYROCK_API UQuestManager: public UObject
{ GENERATED_BODY()

public:
	UQuestManager();
	void Init();

	UFUNCTION(BlueprintCallable)
	void LoadQuestData();

	UFUNCTION(BlueprintCallable)
	void SaveQuestProgress();

	UFUNCTION(BlueprintCallable)
	void LoadQuestProgress();

	UFUNCTION(BlueprintCallable)
	bool IsQuestCompleted() const;

	UFUNCTION(BlueprintCallable)
	void UpdateProgress();

	UFUNCTION(BlueprintCallable)
	FQuestData GetCurrentQuest() const;

protected:
	UPROPERTY()
	UDataTable* QuestDataTable;

	UPROPERTY()
	FQuestData CurrentQuest;

	UPROPERTY()
	int32 CurrentQuestID;

	UPROPERTY()
	int32 CurrentCount;

	UPROPERTY()
	FString SaveSlotName = TEXT("QuestSave");
};
