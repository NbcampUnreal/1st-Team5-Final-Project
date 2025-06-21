// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta=(ExposeOnSpawn))
	FGameplayTag CooldownTag;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* SkillIcon;

	UPROPERTY(meta = (BindWidget))
	UImage* CooldownImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CooldownTimeText;

private:
	UFUNCTION()
	void OnSkillChanged(UTexture2D* IconTexture);

	UFUNCTION()
	void UpdateCooldown(float RemainingTime);
};
