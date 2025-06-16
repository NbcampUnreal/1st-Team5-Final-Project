// PayRockGames


#include "QuestManager.h"
#include "Kismet/GameplayStatics.h"

UQuestManager::UQuestManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> QuestDataObj(TEXT("/Game/DataTable/QuestDataTable.QuestDataTable"));
	if (QuestDataObj.Succeeded())
	{
		QuestDataTable = QuestDataObj.Object;
	}
}

void UQuestManager::Init()
{
	LoadQuestProgress();

	if (QuestDataTable)
	{
		UE_LOG(LogTemp, Log, TEXT("[QuestManager] DataTable Loaded"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuestManager] DataTable NOT FOUND"));
	}

	if (!CurrentQuest.QuestName.IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("[QuestManager] Current Quest: %s"), *CurrentQuest.QuestName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuestManager] Current Quest is EMPTY"));
	}
}

void UQuestManager::LoadQuestData()
{
	
	FString RowName = FString::Printf(TEXT("Quest%d"), CurrentQuestID);
	FQuestData* FoundQuest = QuestDataTable->FindRow<FQuestData>(FName(*RowName), TEXT(""));
	if (FoundQuest)
	{
		CurrentQuest = *FoundQuest;
	}
}

void UQuestManager::SaveQuestProgress()
{
	UQuestSaveGame* SaveGame = nullptr;

	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		SaveGame = Cast<UQuestSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	}
	else
	{
		SaveGame = Cast<UQuestSaveGame>(UGameplayStatics::CreateSaveGameObject(UQuestSaveGame::StaticClass()));
	}

	if (SaveGame)
	{
		SaveGame->CurrentQuestID = CurrentQuestID;
		SaveGame->CurrentProgressCount = CurrentCount;

		const bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveGame, SaveSlotName, 0);
		if (bSuccess)
		{
			UE_LOG(LogTemp, Log, TEXT("[SaveGame] 퀘스트 저장 성공: ID = %d, Count = %d"), CurrentQuestID, CurrentCount);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[SaveGame] 퀘스트 저장 실패!"));
		}
	}
	
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[SaveGame] SaveGame 오브젝트 생성 실패"));
	}
}

void UQuestManager::LoadQuestProgress()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		UQuestSaveGame* LoadedGame = Cast<UQuestSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
		CurrentQuestID = LoadedGame->CurrentQuestID;
		CurrentCount = LoadedGame->CurrentProgressCount;
	}
	else
	{
		CurrentQuestID = 1;
		CurrentCount = 0;
	}

	LoadQuestData();
}

void UQuestManager::UpdateProgress()
{
	CurrentCount++;
	SaveQuestProgress();
}

bool UQuestManager::IsQuestCompleted() const
{
	return CurrentCount >= CurrentQuest.RequiredCount;
}

FQuestData UQuestManager::GetCurrentQuest() const
{
	return CurrentQuest;
}