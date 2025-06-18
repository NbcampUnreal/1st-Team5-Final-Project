// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestData.h"
#include "QuestInfoUI.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UQuestInfoUI : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintCallable)
	void SetQuestData(const FQuestData& Quest);

	//블루프린트함수
	UFUNCTION(BlueprintImplementableEvent, Category = "Quest")
	void CheckItemCollectQuest();

	//래핑함수
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void CallCheckItemCollectQuest();

	//디폴트값 귀찮으니 설정
private:
	FName QuestNameTextName = "Questname";
	FName DescriptionTextName = "DescriptionText";
	FName RequiredCountTextName = "RequiredCountText";
	FName CurrentCountTextName = "CurrentCountText";
	FName TargetTextName = "Targetname";
	
	
	
	
};
