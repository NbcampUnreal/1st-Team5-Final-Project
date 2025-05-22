// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/UI/Widget/BaseUserWidget.h"
#include "StatWindowWidget.generated.h"

UCLASS()
class PAYROCK_API UStatWindowWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UBaseUserWidget* StatContainer;
	
protected:
	virtual void OnWidgetControllerSet() override;
	
};
