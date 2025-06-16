// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Quest/QuestManager.h"
#include "Engine/GameInstance.h"
#include "PRGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UPRGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPRGameInstance();

	
public:
	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	UQuestManager* GetQuestManager() const;

private:
	UPROPERTY()
	UQuestManager* QuestManager;
};
