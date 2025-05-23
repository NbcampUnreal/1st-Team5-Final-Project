// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Character/Blessing/BlessingDataAsset.h"
#include "PayRock/UI/Widget/BaseUserWidget.h"
#include "EquippedBlessingWidget.generated.h"

class UVerticalBox;
class UButton;
class UTextBlock;
class UImage;

UCLASS()
class PAYROCK_API UEquippedBlessingWidget : public UBaseUserWidget
{
	GENERATED_BODY()

protected:
	virtual void OnWidgetControllerSet() override;
	
private:
	UFUNCTION()
	void UpdateActiveBlessing(const FBlessingData& Blessing);
	UFUNCTION()
	void UpdatePassiveBlessing(const FBlessingData& Blessing);
	UFUNCTION()
	void OnActiveRemoveClicked();
	UFUNCTION()
	void OnPassiveRemoveClicked();

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* PassiveBlessingBox;
	UPROPERTY(meta = (BindWidget))
	UImage* PassiveBlessingImage;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PassiveBlessingName;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PassiveBlessingDescription;
	UPROPERTY(meta = (BindWidget))
	UButton* PassiveRemoveButton;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ActiveBlessingBox;
	UPROPERTY(meta = (BindWidget))
	UImage* ActiveBlessingImage;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ActiveBlessingName;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ActiveBlessingDescription;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PenaltyDescription;
	UPROPERTY(meta = (BindWidget))
	UButton* ActiveRemoveButton;
	
};
