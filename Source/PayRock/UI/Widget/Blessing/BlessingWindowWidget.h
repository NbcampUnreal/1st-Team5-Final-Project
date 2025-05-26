// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/UI/Widget/BaseUserWidget.h"
#include "BlessingWindowWidget.generated.h"

/**
 * 
 */
UCLASS(meta = (DisableNativeTick))
class PAYROCK_API UBlessingWindowWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UBaseUserWidget* EquippedBlessingWidget;

	UPROPERTY(meta = (BindWidget))
	UBaseUserWidget* BlessingContainerWidget;

protected:
	virtual void OnWidgetControllerSet() override;
};
