// PayRockGames


#include "PRGameInstance.h"

#include "PayRock/Quest/QuestManager.h"

UPRGameInstance::UPRGameInstance()
{
}

void UPRGameInstance::Init()
{
	Super::Init();
	QuestManager = NewObject<UQuestManager>(this);
	QuestManager->Init();  
}

UQuestManager* UPRGameInstance::GetQuestManager() const
{
	return QuestManager;
}
