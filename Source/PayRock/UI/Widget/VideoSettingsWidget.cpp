// PayRockGames


//#include "UI/Widget/VideoSettingsWidget.h"
#include "VideoSettingsWidget.h"
#include "BaseUserWidget.h"
#include "MainMenuUserWidget.h"
#include "OptionsMenuWidget.h"
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

    if (BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &UVideoSettingsWidget::OnBackClicked);
    }

    // ���� �ػ� ��� �߰�
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
        QualitySlider->SetValue(2.0f); // �⺻�� Medium
    }
}

void UVideoSettingsWidget::OnApplyClicked()
{
    ApplySettings();
}

void UVideoSettingsWidget::OnBackClicked()
{  
    // MainMenu �ٽ� �����ֱ�
    if (MainMenuRef)  //; üũ 
    {
        MainMenuRef->SetVisibility(ESlateVisibility::Visible);

        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        if (PC)
        {
            //; ��ǲ��� ���� 
            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(MainMenuRef->TakeWidget());
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
        }


        //  ������ ���� �����ؼ� ����� -> ������ switcher ������..
        if (OptionsMenuRef)
        {
            OptionsMenuRef->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void UVideoSettingsWidget::ApplySettings()
{
    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
        // �ػ� ����
        FString SelectedResolution = ResolutionComboBox->GetSelectedOption();
        FIntPoint Res = ParseResolution(SelectedResolution);
        Settings->SetScreenResolution(Res);

        // ��üȭ�� ���
        FString SelectedMode = FullscreenComboBox->GetSelectedOption();
        EWindowMode::Type WindowMode = EWindowMode::Fullscreen;
        if (SelectedMode == TEXT("Windowed")) WindowMode = EWindowMode::Windowed;
        else if (SelectedMode == TEXT("Borderless")) WindowMode = EWindowMode::WindowedFullscreen;

        Settings->SetFullscreenMode(WindowMode);

        // ǰ�� ����
        int32 QualityLevel = FMath::RoundToInt(QualitySlider->GetValue());
        Settings->SetOverallScalabilityLevel(QualityLevel);

        // ����
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