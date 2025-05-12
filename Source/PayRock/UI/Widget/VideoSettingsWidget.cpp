// PayRockGames


//#include "UI/Widget/VideoSettingsWidget.h"
#include "BaseUserWidget.h"
#include "VideoSettingsWidget.h"

#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"


void UVideoSettingsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ApplyButton)
    {
        ApplyButton->OnClicked.AddDynamic(this, &UVideoSettingsWidget::OnApplyClicked);
    }

    // 예시 해상도 목록 추가
    if (ResolutionComboBox)
    {
        ResolutionComboBox->AddOption(TEXT("1920x1080"));
        ResolutionComboBox->AddOption(TEXT("1600x900"));
        ResolutionComboBox->AddOption(TEXT("1280x720"));
        ResolutionComboBox->SetSelectedIndex(0);
    }

    if (FullscreenComboBox)
    {
        FullscreenComboBox->AddOption(TEXT("Fullscreen"));
        FullscreenComboBox->AddOption(TEXT("Windowed"));
        FullscreenComboBox->AddOption(TEXT("Borderless"));
        FullscreenComboBox->SetSelectedIndex(0);
    }

    if (QualitySlider)
    {
        QualitySlider->SetMinValue(0.0f);
        QualitySlider->SetMaxValue(3.0f);
        QualitySlider->SetValue(2.0f); // 기본값 Medium
    }
}

void UVideoSettingsWidget::OnApplyClicked()
{
    ApplySettings();
}

void UVideoSettingsWidget::ApplySettings()
{
    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
        // 해상도 적용
        FString SelectedResolution = ResolutionComboBox->GetSelectedOption();
        FIntPoint Res = ParseResolution(SelectedResolution);
        Settings->SetScreenResolution(Res);

        // 전체화면 모드
        FString SelectedMode = FullscreenComboBox->GetSelectedOption();
        EWindowMode::Type WindowMode = EWindowMode::Fullscreen;
        if (SelectedMode == TEXT("Windowed")) WindowMode = EWindowMode::Windowed;
        else if (SelectedMode == TEXT("Borderless")) WindowMode = EWindowMode::WindowedFullscreen;

        Settings->SetFullscreenMode(WindowMode);

        // 품질 설정
        int32 QualityLevel = FMath::RoundToInt(QualitySlider->GetValue());
        Settings->SetOverallScalabilityLevel(QualityLevel);

        // 적용
        Settings->ApplySettings(false);
        Settings->SaveSettings();
    }
}

FIntPoint UVideoSettingsWidget::ParseResolution(const FString& ResolutionString) const
{
    FString Left, Right;
    if (ResolutionString.Split(TEXT("x"), &Left, &Right))
    {
        return FIntPoint(FCString::Atoi(*Left), FCString::Atoi(*Right));
    }
    return FIntPoint(1920, 1080); // fallback
}