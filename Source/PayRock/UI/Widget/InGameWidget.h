// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseUserWidget.h"
#include "PayRock/UI/WidgetController/OverlayWidgetController.h"
#include "InGameWidget.generated.h"

UCLASS()
class PAYROCK_API UInGameWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	virtual void OnWidgetControllerSet() override;
	void UpdateCleanPercent();
	
private:
	UPROPERTY(meta = (BindWidgetOptional))
	UBaseUserWidget* HealthBar;
	UPROPERTY(meta = (BindWidgetOptional))
	UBaseUserWidget* ManaBar;

	UPROPERTY(meta = (BindWidgetOptional))
	UBaseUserWidget* BlessingSkill;
	UPROPERTY(meta = (BindWidgetOptional))
	UBaseUserWidget* AccessorySkill;
	UPROPERTY(meta = (BindWidgetOptional))
	UBaseUserWidget* WeaponSkill;

	UPROPERTY(meta = (BindWidgetOptional))
	UBaseUserWidget* NecroExpel;
	UPROPERTY(meta = (BindWidgetOptional))
	UBaseUserWidget* NecroSlow;
	UPROPERTY(meta = (BindWidgetOptional))
	UBaseUserWidget* NecroBlind;

	UPROPERTY(meta = (BindWidgetOptional))
	class UCleanPercent* CleanPercent;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_RemainingTime;

	UPROPERTY(meta = (BindWidget))
	class UBorder* NotificationBackground;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NotificationText;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* NotificationAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Notification")
	TMap<ENotificationType, FText> NotificationMessages;

	FTimerHandle UpdateTimerHandle;

	UFUNCTION()
	void DisplayNotification(ENotificationType Type);
	void UpdateTimeInfo();
	
};
