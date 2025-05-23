// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Character/Blessing/BlessingData.h"
#include "PayRock/UI/Widget/BaseUserWidget.h"
#include "BlessingRowWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;

UCLASS()
class PAYROCK_API UBlessingRowWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetBlessingImage(UTexture2D* ImageTexture);
	UFUNCTION(BlueprintCallable)
	void SetBlessingName(const FText& Text);
	UFUNCTION(BlueprintCallable)
	void SetBlessing(const FBlessingData& Blessing);
	UFUNCTION(BlueprintCallable)
	void SetInfoText(const FText& Text);

protected:
	virtual void OnWidgetControllerSet() override;
	
private:
	UFUNCTION()
	void NotifyBlessingSelected();
	
	UPROPERTY(meta = (BindWidget))
	UImage* BlessingImage;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BlessingName;
	UPROPERTY(meta = (BindWidget))
	UButton* OverlayButton;
	UPROPERTY()
	FBlessingData CachedBlessing;
};
