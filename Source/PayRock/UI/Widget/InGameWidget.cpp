// PayRockGames


#include "InGameWidget.h"
#include "PayRock/GameSystem/PRGameState.h"
#include "Components/TextBlock.h"
#include "HealthBar.h"
#include "ManaBar.h"

void UInGameWidget::OnWidgetControllerSet()
{
	Super::OnWidgetControllerSet();


	// UI �ʱ� ǥ��
	UpdateTimeInfo();

	// 1�ʸ��� ���� (Ÿ�̸� ����)
	GetWorld()->GetTimerManager().SetTimer(
		UpdateTimerHandle,
		this,
		&UInGameWidget::UpdateTimeInfo,
		1.0f,
		true
	);

	// ���� ���� ��Ʈ�ѷ� !!!!! �߿� (GAS ���� �޾ƿ����� ���)
	HealthBar->SetWidgetController(WidgetController);
	ManaBar->SetWidgetController(WidgetController);
}

void UInGameWidget::UpdateTimeInfo()
{
	if (APRGameState* GS = GetWorld()->GetGameState<APRGameState>())
	{
		 const int32 RemainingTime = GS->GetRemainingTime();
		 const int32 ExtractionTime = GS->GetExtractionActivationTime();

		// ���� ��ġ �ð� ǥ��
		if (Text_RemainingTime)
		{
			const FString TimeStr = FString::Printf(TEXT("%d"), RemainingTime);
			Text_RemainingTime->SetText(FText::FromString(TimeStr));
		}

		// Ż�� �ð� ǥ��
		if (Text_ExtractionTime)
		{
			const FString ExtractionStr = FString::Printf(TEXT("%d"), ExtractionTime);
			Text_ExtractionTime->SetText(FText::FromString(ExtractionStr));
		}
	}

}
