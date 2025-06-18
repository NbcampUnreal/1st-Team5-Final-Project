// PayRockGames


#include "QuestInfoUI.h"

#include "Components/TextBlock.h"
#include "PayRock/GameSystem/PRAdvancedGameInstance.h"
#include "PayRock/GameSystem/PRGameInstance.h"

void UQuestInfoUI::SetQuestData(const FQuestData& Quest)
{

	CheckItemCollectQuest();

	if (UTextBlock* QuestNameText = Cast<UTextBlock>(GetWidgetFromName(QuestNameTextName)))
	{
	}
	if (UTextBlock* DescText = Cast<UTextBlock>(GetWidgetFromName(DescriptionTextName)))
	{
		DescText->SetText(FText::FromString(Quest.QuestDescription));
	}

	if (UTextBlock* RequireCountText = Cast<UTextBlock>(GetWidgetFromName(RequiredCountTextName)))
	{
		FString CountString = FString::Printf(TEXT("%d"), Quest.RequiredCount);
		RequireCountText->SetText(FText::FromString(CountString));
	}
	if (UTextBlock* TargetText= Cast<UTextBlock>(GetWidgetFromName(TargetTextName)))
	{
		TargetText->SetText(FText::FromString(Quest.TargetName));
	}
	if (UTextBlock* CurrentCountText = Cast<UTextBlock>(GetWidgetFromName(CurrentCountTextName)))
	{
		UPRAdvancedGameInstance* GI = Cast<UPRAdvancedGameInstance>(GetGameInstance());
		if (GI)
		{
			UQuestManager* QM = GI->GetQuestManager();
			if (QM)
			{
					int32 currentcount = QM->GetCurrentCount();
					FString CountString = FString::Printf(TEXT("%d"), currentcount);
					CurrentCountText->SetText(FText::FromString(CountString));
			}
		}
	}


}
