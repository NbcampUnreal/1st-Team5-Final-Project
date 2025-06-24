// PayRockGames


#include "CleanPercent.h"

#include "Components/TextBlock.h"
#include "PayRock/GameSystem/PRGameState.h"


void UCleanPercent::SetCleanData()
{
	APRGameState* PRGS = GetWorld()->GetGameState<APRGameState>();
	if (!PRGS) return;

	float Percent = PRGS->GetDeadPercent();
	int32 Dead = PRGS->GetDieMonsterCount();
	int32 Total = PRGS->GetTotalMonsterCount();

	if (UTextBlock* PercentText = Cast<UTextBlock>(GetWidgetFromName(CleanPercent)))
	{
		FString Display = FString::Printf(TEXT("%d / %d (%.0f%%)"), Dead, Total, Percent * 100.f);
		PercentText->SetText(FText::FromString(Display));
	}

	
	if (UTextBlock* CleanMessage = Cast<UTextBlock>(GetWidgetFromName(CleanTextMessage)))
	{
		//퍼센트가 일정량 되었을때마다 다르게 50퍼
		if (Percent>= 0.5)
		{
			CleanMessage->SetText(FText::FromString(TEXT("어둠이 걷혀져 맑아진 기분이다")));
		}
		else if (Percent>= 0.8)
		{
			CleanMessage->SetText(FText::FromString(TEXT("이 빌어먹을 저주를 거의 다 끝내간다")));
		}
		else if (Percent==1)
		{
			CleanMessage->SetText(FText::FromString(TEXT("마을의 정중앙에 특별한 보상이 출현하였다.")));
		}
	}
}
