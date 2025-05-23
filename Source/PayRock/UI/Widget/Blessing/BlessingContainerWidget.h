// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/UI/Widget/BaseUserWidget.h"
#include "BlessingContainerWidget.generated.h"

class UScrollBox;

UCLASS(meta = (DisableNativeTick))
class PAYROCK_API UBlessingContainerWidget : public UBaseUserWidget
{
	GENERATED_BODY()

protected:
	virtual void OnWidgetControllerSet() override;
	
	
private:
	void InitializeBlessingRows();

	UPROPERTY(meta = (BindWidget))
	UScrollBox* PassiveBlessingBox;
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ActiveBlessingBox;
};
