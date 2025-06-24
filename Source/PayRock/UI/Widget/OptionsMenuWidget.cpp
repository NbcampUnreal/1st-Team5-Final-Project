// PayRockGames


//#include "UI/Widget/OptionsMenuWidget.h"
#include "OptionsMenuWidget.h"
#include "VideoSettingsWidget.h"
#include "AudioSettingsWidget.h"
#include "BaseUserWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "MainMenuUserWidget.h"
#include "PayRock/Player/PRPlayerController.h"

void UOptionsMenuWidget::OnVideoButtonClicked()
{
    SwitchTab(0);

}

void UOptionsMenuWidget::OnAudioButtonClicked()
{
    SwitchTab(1);

}

void UOptionsMenuWidget::OnInputButtonClicked()
{
    SwitchTab(2);

}

void UOptionsMenuWidget::OnGameplayButtonClicked()
{
    SwitchTab(3);

}

void UOptionsMenuWidget::SwitchTab(int32 TabIndex)
{
    if (TabSwitcher && TabSwitcher->GetNumWidgets() > TabIndex)
    {
        TabSwitcher->SetActiveWidgetIndex(TabIndex);
    }
}

void UOptionsMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    
    if (VideoButton)
    {
        VideoButton->OnClicked.AddDynamic(this, &UOptionsMenuWidget::OnVideoButtonClicked);
    }

    if (AudioButton)
    {
        AudioButton->OnClicked.AddDynamic(this, &UOptionsMenuWidget::OnAudioButtonClicked);
    }

    if (InputButton)
    {
        InputButton->OnClicked.AddDynamic(this, &UOptionsMenuWidget::OnInputButtonClicked);
    }

    if (GameplayButton)
    {
        GameplayButton->OnClicked.AddDynamic(this, &UOptionsMenuWidget::OnGameplayButtonClicked);
    }

    SwitchTab(0);
}

void UOptionsMenuWidget::InitOptionsMenu(UMainMenuUserWidget* InMainMenuRef)
{
    MainMenuRef = InMainMenuRef;

    if (VideoSettingsWidget && MainMenuRef)
    {
        VideoSettingsWidget->MainMenuRef = MainMenuRef;
        VideoSettingsWidget->OptionsMenuRef = this;
    }

    if (AudioSettingsWidget && MainMenuRef)
    {
        AudioSettingsWidget->MainMenuRef = MainMenuRef;
        AudioSettingsWidget->OptionsMenuRef = this;

    }
    // ; todo; 
    // ����  Ű�� ������ �鵵�� ����

    // ���� ���� �÷���

}


FReply UOptionsMenuWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    if (InKeyEvent.GetKey() == EKeys::Escape)
    {
        if (MainMenuRef)
        {
            MainMenuRef->SetVisibility(ESlateVisibility::Visible);
            RemoveFromParent();

            APlayerController* PC = GetWorld()->GetFirstPlayerController();
            if (PC)
            {
                FInputModeUIOnly InputMode;
                InputMode.SetWidgetToFocus(MainMenuRef->TakeWidget());
                InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                PC->SetInputMode(InputMode);
                PC->bShowMouseCursor = true;
            }

            return FReply::Handled();
        }

        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        if (PC)
        {
            if (APRPlayerController* PRPC = Cast<APRPlayerController>(PC))
            {
                PRPC->ToggleSettingsMenu();
                return FReply::Handled();
            }
        }
    }

    return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UOptionsMenuWidget::NativeConstruct()
{
    SetIsFocusable(true);
}
