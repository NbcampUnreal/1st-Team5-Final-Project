// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "SkillData.h"
#include "PayRock/UI/Widget/BaseUserWidget.h"
#include "SkillIconWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class PAYROCK_API USkillIconWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	virtual void OnWidgetControllerSet() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	ESkillItem SkillItem;
	
	UPROPERTY(meta = (BindWidget))
	UImage* SkillIcon;

	UPROPERTY(meta = (BindWidget))
	UImage* CooldownImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CooldownTimeText;

private:
	UFUNCTION()
	void OnSkillChanged(const FSkillData& SkillData);

	UFUNCTION()
	void UpdateCooldown(float RemainingTime);
};
