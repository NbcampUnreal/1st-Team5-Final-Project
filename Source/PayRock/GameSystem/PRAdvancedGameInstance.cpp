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
}

void UPRAdvancedGameInstance::Shutdown()
{
	Super::Shutdown();
	if (QuestManager)
	{
		QuestManager->SaveQuestProgress();
		UE_LOG(LogTemp, Log, TEXT("[GameInstance] 게임 종료 - 퀘스트 자동 저장 완료"));
	}
}

UQuestManager* UPRAdvancedGameInstance::GetQuestManager() const
{
	return QuestManager;
}
