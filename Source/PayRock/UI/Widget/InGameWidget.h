// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseUserWidget.h"
#include "InGameWidget.generated.h"
class UHealthBar;
class UManaBar;

UCLASS()
class PAYROCK_API UInGameWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	virtual void OnWidgetControllerSet() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	UHealthBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	UManaBar* ManaBar;
	UPROPERTY(meta = (BindWidget))
	UBaseUserWidget* BlessingIcon;


	// ���� �ð� UI
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_RemainingTime;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_ExtractionTime;

	FTimerHandle UpdateTimerHandle;

	// �ֱ��� ���� �Լ�
	void UpdateTimeInfo();
};
