// PayRockGames


#include "PRAdvancedGameInstance.h"

#include "PayRock/Quest/QuestManager.h"


void UPRAdvancedGameInstance::Init()
{
	Super::Init();
	
	if (QuestManagerClass)
	{
		QuestManager = NewObject<UQuestManager>(this, QuestManagerClass);
		QuestManager->Init();
	}

	FCoreDelegates::OnPreExit.AddUObject(this, &UPRAdvancedGameInstance::OnPreExitHandler);

}

void UPRAdvancedGameInstance::Shutdown()
{
	Super::Shutdown();
	if (QuestManager)
	{
		QuestManager->SaveQuestProgress();
		QuestManager->ClearTimer();
		
		UE_LOG(LogTemp, Log, TEXT("[GameInstance] Shutdown - 퀘스트 자동 저장 완료"));
	}
}

UQuestManager* UPRAdvancedGameInstance::GetQuestManager() const
{
	return QuestManager;
}

void UPRAdvancedGameInstance::OnPreExitHandler()
{
	if (QuestManager)
	{
		QuestManager->SaveQuestProgress();
		UE_LOG(LogTemp, Log, TEXT("[GameInstance] OnPreExitHandler - 퀘스트 자동 저장 완료"));
	}
}
