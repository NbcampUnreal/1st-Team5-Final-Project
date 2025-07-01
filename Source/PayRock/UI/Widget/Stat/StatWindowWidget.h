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

	UPROPERTY(meta = (BindWidget))
	UBaseUserWidget* BlessingSkillInfo;
	UPROPERTY(meta = (BindWidget))
	UBaseUserWidget* AccessorySkillInfo;
	UPROPERTY(meta = (BindWidget))
	UBaseUserWidget* WeaponSkillInfo;
	
protected:
	virtual void OnWidgetControllerSet() override;
	
};
