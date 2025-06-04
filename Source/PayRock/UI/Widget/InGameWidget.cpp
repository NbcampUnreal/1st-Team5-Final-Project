// PayRockGames


#include "InGameWidget.h"
#include "PayRock/GameSystem/PRGameState.h"
#include "Components/TextBlock.h"
#include "HealthBar.h"
#include "ManaBar.h"

void UInGameWidget::OnWidgetControllerSet()
{
	Super::OnWidgetControllerSet();


	// UI 초기 표시
	UpdateTimeInfo();

	// 1초마다 갱신 (타이머 설정)
	GetWorld()->GetTimerManager().SetTimer(
		UpdateTimerHandle,
		this,
		&UInGameWidget::UpdateTimeInfo,
		1.0f,
		true
	);

	// 개인 위젯 컨트롤러 !!!!! 중요 (GAS 에서 받아오도록 등록)
	HealthBar->SetWidgetController(WidgetController);
	ManaBar->SetWidgetController(WidgetController);
}

void UInGameWidget::UpdateTimeInfo()
{
	if (APRGameState* GS = GetWorld()->GetGameState<APRGameState>())
	{
		 const int32 RemainingTime = GS->GetRemainingTime();
		 const int32 ExtractionTime = GS->GetExtractionActivationTime();

		// 남은 매치 시간 표시
		if (Text_RemainingTime)
		{
			const FString TimeStr = FString::Printf(TEXT("%d"), RemainingTime);
			Text_RemainingTime->SetText(FText::FromString(TimeStr));
		}

		// 탈출 시간 표시
		if (Text_ExtractionTime)
		{
			const FString ExtractionStr = FString::Printf(TEXT("%d"), ExtractionTime);
			Text_ExtractionTime->SetText(FText::FromString(ExtractionStr));
		}
	}

}
