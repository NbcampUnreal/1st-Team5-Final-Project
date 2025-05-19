// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseUserWidget.h"
#include "InGameWidget.generated.h"


UCLASS()
class PAYROCK_API UInGameWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	virtual void OnWidgetControllerSet() override;
	
private:
	UPROPERTY(meta = (BindWidgetOptional))
	UBaseUserWidget* HealthBar;
	// UPROPERTY(meta = (BindWidgetOptional))
	// UBaseUserWidget* ManaBar;
};
