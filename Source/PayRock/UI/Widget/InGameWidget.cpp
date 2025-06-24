// PayRockGames


#include "InGameWidget.h"
#include "PayRock/GameSystem/PRGameState.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

void UInGameWidget::OnWidgetControllerSet()
{
	Super::OnWidgetControllerSet();

	GetWorld()->GetTimerManager().SetTimer(
		UpdateTimerHandle,
		this,
		&UInGameWidget::UpdateTimeInfo,
		0.5f,
		true
	);

	if (HealthBar) { HealthBar->SetWidgetController(WidgetController); }
	if (ManaBar) { ManaBar->SetWidgetController(WidgetController); }
	if (BlessingSkill) { BlessingSkill->SetWidgetController(WidgetController); }
	if (AccessorySkill) { AccessorySkill->SetWidgetController(WidgetController); }
	if (WeaponSkill) { WeaponSkill->SetWidgetController(WidgetController); }
	if (NecroExpel) { NecroExpel->SetWidgetController(WidgetController); }
	if (NecroSlow) { NecroSlow->SetWidgetController(WidgetController); }
	if (NecroBlind) { NecroBlind->SetWidgetController(WidgetController); }

	if (UOverlayWidgetController* OverlayWC = Cast<UOverlayWidgetController>(WidgetController))
	{
		OverlayWC->OnNotificationRequest.AddUniqueDynamic(this, &UInGameWidget::DisplayNotification);
	}
}

void UInGameWidget::DisplayNotification(ENotificationType Type)
{
	NotificationBackground->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	NotificationText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (FText* Text = NotificationMessages.Find(Type))
	{
		NotificationText->SetText(*Text);
	}
	PlayAnimation(NotificationAnim);
}

void UInGameWidget::UpdateTimeInfo()
{
	if (APRGameState* GS = GetWorld()->GetGameState<APRGameState>())
	{
		const int32 RemainingTime = GS->GetRemainingTime();

		if (Text_RemainingTime)
		{
			const FString TimeStr = FString::Printf(TEXT("%d"), RemainingTime);
			Text_RemainingTime->SetText(FText::FromString(TimeStr));
		}
	}
}
