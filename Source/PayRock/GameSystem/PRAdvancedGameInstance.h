// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Quest/QuestManager.h"
#include "AdvancedFriendsGameInstance.h"
#include "PRAdvancedGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UPRAdvancedGameInstance : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()


	
	
public:
	virtual void Init() override;
	
	virtual void Shutdown() override;

	UFUNCTION(BlueprintCallable)
	UQuestManager* GetQuestManager() const;

	void OnPreExitHandler();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Quest")
	UQuestManager* QuestManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TSubclassOf<UQuestManager> QuestManagerClass;

};
