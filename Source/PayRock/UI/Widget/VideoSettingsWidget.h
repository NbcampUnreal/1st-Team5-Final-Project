// PayRockGames

#pragma once

#include "CoreMinimal.h"
//#include "UI/Widget/BaseUserWidget.h"
#include "BaseUserWidget.h"
#include "VideoSettingsWidget.generated.h"

class UComboBoxString;
class UButton;
class UMainMenuUserWidget;
class UOptionsMenuWidget;


UCLASS()
class PAYROCK_API UVideoSettingsWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
protected:
	UFUNCTION()
	void OnApplyClicked();

	UFUNCTION()
	void OnBackClicked();

	void InitResolutionOptions(const FIntPoint& DesktopRes, const FIntPoint& CurrentRes);
	void InitWindowModeOptions(EWindowMode::Type CurrentMode);
	void InitQualityOptions(int32 CurrentQuality);
	void ApplySettings();

	FIntPoint ParseResolution(const FString& ResolutionString) const;

public:
	UPROPERTY()
	UMainMenuUserWidget* MainMenuRef;

	UPROPERTY()
	UOptionsMenuWidget* OptionsMenuRef;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Resolutions")
	TArray<FIntPoint> SupportedResolutions;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Quality")
	TArray<FString> SupportedQualities;
	
protected:
    UPROPERTY(meta = (BindWidget))
    UComboBoxString* ResolutionComboBox;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* FullscreenComboBox;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* QualityComboBox;

    UPROPERTY(meta = (BindWidget))
    UButton* ApplyButton;

    UPROPERTY(meta = (BindWidgetOptional))
    UButton* BackButton;
};
