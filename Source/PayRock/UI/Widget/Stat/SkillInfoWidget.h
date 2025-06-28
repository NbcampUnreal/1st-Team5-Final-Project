// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Character/Blessing/BlessingData.h"
#include "PayRock/UI/Widget/Skill/SkillData.h"
#include "PayRock/UI/Widget/BaseUserWidget.h"
#include "SkillInfoWidget.generated.h"

class UImage;
class UTextBlock;

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Blessing,
	Accessory,
	Weapon,
	MAX
};

UCLASS()
class PAYROCK_API USkillInfoWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	virtual void OnWidgetControllerSet() override;
	
	UFUNCTION()
	void SetBlessingSkillInfo(const FBlessingData& BlessingData);
	UFUNCTION()
	void SetItemSkillInfo(const FSkillData& SkillData);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta=(ExposeOnSpawn))
	ESkillType Type;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SkillType;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SkillName;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SkillDescription;
	UPROPERTY(meta = (BindWidget))
	UImage* SkillImage;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* PenaltyDescription;
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* Cooldown;
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* ManaCost;
};
