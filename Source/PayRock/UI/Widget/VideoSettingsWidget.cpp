// PayRockGames


//#include "UI/Widget/VideoSettingsWidget.h"
#include "VideoSettingsWidget.h"
#include "MainMenuUserWidget.h"
#include "OptionsMenuWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/Button.h"
#include "GameFramework/GameUserSettings.h"

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

    UGameUserSettings* Settings = GEngine->GetGameUserSettings();
    if (ResolutionComboBox)
    {
        if (Settings)
        {
            FIntPoint DesktopRes = Settings->GetDesktopResolution();
            FIntPoint CurrentRes = Settings->GetScreenResolution();
            InitResolutionOptions(DesktopRes, CurrentRes);
        }
    }

    if (FullscreenComboBox)
    {
        if (Settings)
        {
            EWindowMode::Type WindowMode = Settings->GetFullscreenMode();
            InitWindowModeOptions(WindowMode);
        }
    }

    if (QualityComboBox)
    {
        if (Settings)
        {
            int32 QualityLevel = Settings->GetOverallScalabilityLevel();
            InitQualityOptions(CurrentQuality);
        }
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

void UVideoSettingsWidget::InitResolutionOptions(const FIntPoint& DesktopRes, const FIntPoint& CurrentRes)
{
    if (!ResolutionComboBox) return;
    
    ResolutionComboBox->ClearOptions();
    
    int CurrentResIndex = -1;
    int ComboBoxIndex = 0;
    for (const auto& Resolution : SupportedResolutions)
    {
        if (DesktopRes.X < Resolution.X || DesktopRes.Y < Resolution.Y) continue;
        ResolutionComboBox->AddOption(FString::Printf(TEXT("%dX%d"), Resolution.X, Resolution.Y));
        if (CurrentRes.X == Resolution.X && CurrentRes.Y == Resolution.Y)
        {
            CurrentResIndex = ComboBoxIndex; 
        }
        ComboBoxIndex++;
    }
    if (CurrentResIndex == -1 && ComboBoxIndex > 0)
    {
        CurrentResIndex = 0;
    }
    ResolutionComboBox->SetSelectedIndex(CurrentResIndex);
}

void UVideoSettingsWidget::InitWindowModeOptions(EWindowMode::Type CurrentMode)
{
    if (!FullscreenComboBox) return;
    FullscreenComboBox->ClearOptions();
    FullscreenComboBox->AddOption(TEXT("전체화면"));
    FullscreenComboBox->AddOption(TEXT("창 모드"));
    FullscreenComboBox->AddOption(TEXT("창 모드 전체화면"));
    
    switch (CurrentMode)
    {
    case EWindowMode::Fullscreen:
        FullscreenComboBox->SetSelectedIndex(0);
        break;
    case EWindowMode::Windowed:
        FullscreenComboBox->SetSelectedIndex(1);
        break;
    case EWindowMode::WindowedFullscreen:
        FullscreenComboBox->SetSelectedIndex(2);
        break;
    default:
        FullscreenComboBox->SetSelectedIndex(2);
    }
}

void UVideoSettingsWidget::InitQualityOptions(int32 CurrentQuality)
{
    // 0:low, 1:medium, 2:high, 3:epic, 4:cinematic (not supported)
    if (!QualityComboBox) return;
    
    QualityComboBox->ClearOptions();
    for (const FString& Quality : SupportedQualities)
    {
        QualityComboBox->AddOption(Quality);
    }
    QualityComboBox->SetSelectedIndex(CurrentQuality);
}

void UVideoSettingsWidget::ApplySettings()
{
    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
        FString SelectedResolution = ResolutionComboBox->GetSelectedOption();
        FIntPoint Res = ParseResolution(SelectedResolution);
        Settings->SetScreenResolution(Res);
        
        FString SelectedMode = FullscreenComboBox->GetSelectedOption();
        EWindowMode::Type WindowMode = EWindowMode::Fullscreen;
        if (SelectedMode == TEXT("창 모드")) WindowMode = EWindowMode::Windowed;
        else if (SelectedMode == TEXT("창 모드 전체화면")) WindowMode = EWindowMode::WindowedFullscreen;
        Settings->SetFullscreenMode(WindowMode);
        
        FString SelectedQuality = QualityComboBox->GetSelectedOption();
        int32 Index = QualityComboBox->FindOptionIndex(SelectedQuality);
        Settings->SetOverallScalabilityLevel(Index >= 0 && Index <= 3 ? Index : 0);
        
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