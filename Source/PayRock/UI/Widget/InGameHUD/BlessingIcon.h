// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Character/Blessing/BlessingData.h"
#include "PayRock/UI/Widget/BaseUserWidget.h"
#include "BlessingIcon.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class PAYROCK_API UBlessingIcon : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	virtual void OnWidgetControllerSet() override;

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* BlessingIcon;

	UPROPERTY(meta = (BindWidget))
	UImage* CooldownImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CooldownTimeText;

private:
	UFUNCTION()
	void OnActiveBlessingChanged(const FBlessingData& Blessing);

	UFUNCTION()
	void UpdateCooldown(float RemainingTime);
};
