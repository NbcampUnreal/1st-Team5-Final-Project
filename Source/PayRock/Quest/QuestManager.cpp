// PayRockGames


#include "QuestManager.h"

#include "QuestInfoUI.h"
#include "Blueprint/UserWidget.h"
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
		UE_LOG(LogTemp, Log, TEXT("[QuestManager] 퀘스트데이터테이블 연동"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuestManager] 퀘스트데이터테이블 읎다"));
	}


	if (!QuestWidget && QuestWidgetClass)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(this))
		{
			QuestWidget = CreateWidget<UQuestInfoUI>(World, QuestWidgetClass);
			if (QuestWidget)
			{
				QuestWidget->SetQuestData(CurrentQuest);
				QuestWidget->SetVisibility(ESlateVisibility::Visible);
				QuestWidget->AddToViewport();
		UE_LOG(LogTemp, Warning, TEXT("[QuestManager] 퀘스트위젯 생성"));
			}
		}
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

	// 만약 퀘스트번호를 넘기면
	else
	{
		TArray<FName> RowNames = QuestDataTable->GetRowNames();
		if (RowNames.Num() > 0)
		{
			FName LastRowName = RowNames.Last();
			FQuestData* LastQuest = QuestDataTable->FindRow<FQuestData>(LastRowName, TEXT(""));
			if (LastQuest)
			{
				CurrentQuest = *LastQuest;

				FString LastRowStr = LastRowName.ToString(); 
				FString LastIDStr;
				LastRowStr.Split(TEXT("Quest"), nullptr, &LastIDStr); 
				CurrentQuestID = FCString::Atoi(*LastIDStr);
				
				UE_LOG(LogTemp, Warning, TEXT("[QuestManager] 저장된 QuestID가 잘못되어 마지막 퀘스트로 대체: ID = %d"), CurrentQuestID);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[QuestManager] 데이터 테이블에 퀘스트가 없습니다!"));
		}
	}
}

void UQuestManager::SaveQuestProgress()
{
	 SaveGame = nullptr;

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
	if (QuestWidget)
	{
		QuestWidget->SetQuestData(CurrentQuest);
	}
}

bool UQuestManager::IsQuestCompleted() const
{
	return CurrentCount >= CurrentQuest.RequiredCount;
}

void UQuestManager::NextQuest()
{
	CurrentQuestID++;
	CurrentCount = 0;
	SaveQuestProgress();
	LoadQuestData();
	QuestWidget->SetQuestData(CurrentQuest);
}

FQuestData UQuestManager::GetCurrentQuest() const
{
	return CurrentQuest;
}

UQuestSaveGame* UQuestManager::GetSaveQuest() const
{
	return SaveGame;
}

int32 UQuestManager::GetCurrentCount() const
{
	return CurrentCount;
}

void UQuestManager::ToggleQuestUI()
{
	if (!QuestWidget) return;

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	
	if (QuestWidget->IsVisible())
	{
		QuestWidget->SetVisibility(ESlateVisibility::Hidden);
		// if (PC)
		// {
		// 	PC->SetShowMouseCursor(false);
		// 	PC->SetInputMode(FInputModeGameOnly());
		// }
		UE_LOG(LogTemp, Error, TEXT("[QuestManager] 토글퀘스트 숨기기"));
	}
	else
	{
		if (!QuestWidget->IsInViewport())
		{
			QuestWidget->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("[QuestManager] Viewport에 위젯 추가"));
		}
			QuestWidget->SetQuestData(CurrentQuest);
			QuestWidget->SetVisibility(ESlateVisibility::Visible);

		// if (PC)
		// {
		// 	FInputModeGameAndUI InputMode;
		// 	InputMode.SetWidgetToFocus(QuestWidget->TakeWidget());
		// 	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		// 	PC->SetInputMode(InputMode);
		// 	PC->SetShowMouseCursor(true);
		// }
			UE_LOG(LogTemp, Error, TEXT("[QuestManager] 토글퀘스트 보이기"));
	}
}
