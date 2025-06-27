// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestInfoUI.h"
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
	void NextQuest();
	UFUNCTION(BlueprintCallable)
	void UpdateProgress();
	UFUNCTION(BlueprintCallable)
	void SetCurrentCount(int value);
	UFUNCTION(BlueprintCallable)
	void ResetCurrentCount();

	UFUNCTION(BlueprintCallable)
	FQuestData GetCurrentQuest() const;

	UFUNCTION(BlueprintCallable)
	UQuestSaveGame* GetSaveQuest() const;

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentCount() const;
	
	UFUNCTION(BlueprintCallable)
	void ToggleQuestUI();

	void ClearTimer();

	
protected:

	UPROPERTY()
	UQuestSaveGame* SaveGame;
	UPROPERTY()
	UDataTable* QuestDataTable;

	UPROPERTY()
	FQuestData CurrentQuest;

	UPROPERTY()
	int32 CurrentQuestID;

	UPROPERTY()
	int32 CurrentCount = 0;

	UPROPERTY()
	FString SaveSlotName = TEXT("QuestSave");

public:
	UPROPERTY()
	UQuestInfoUI* QuestWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI") 
	TSubclassOf<UUserWidget> QuestWidgetClass;
	FTimerHandle AutoSaveTimerHandle;
};
