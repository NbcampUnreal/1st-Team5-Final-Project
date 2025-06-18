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
    // �� ����ó�� ��ȿ����(��������) && TabIndex�� ��ȿ����(ex) 4���� �����̸� 0,1,2,3 �� ��ȿ�ϴ�.
    if (TabSwitcher && TabSwitcher->GetNumWidgets(/*�𸮾������Լ�*/) > TabIndex)
    {
        // �ɼ� �� �ٲٱ� 
        TabSwitcher->/*UE����*/SetActiveWidgetIndex(TabIndex);
    }
}

void UOptionsMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();


    //// Back �������� ���ư� �� �ֵ��� ��� �ֱ� 
    //if (VideoSettingsWidget && MainMenuRef)
    //{
    //    VideoSettingsWidget->MainMenuRef = MainMenuRef;
    //}

    //if (AudioSettingsWidget && MainMenuRef)
    //{
    //    VideoSettingsWidget->MainMenuRef = MainMenuRef;
    //}



    // ; �Լ��� ����
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

    // �⺻ �� ���� (���� ��)
    SwitchTab(0);


    //// Back �������� ���ư� �� �ֵ��� ��� �ְ� �ּ� �Ѱ��ֱ� 
    //if (VideoSettingsWidget && MainMenuRef)
    //{
    //    VideoSettingsWidget->MainMenuRef = MainMenuRef;
    //}

    //if (AudioSettingsWidget && MainMenuRef)
    //{
    //    VideoSettingsWidget->MainMenuRef = MainMenuRef;
    //}

}

void UOptionsMenuWidget::InitOptionsMenu(UMainMenuUserWidget* InMainMenuRef)
{
    MainMenuRef = InMainMenuRef;

    // ���� ���� �ǿ��� ��� �ְ�
    if (VideoSettingsWidget && MainMenuRef)
    {
        VideoSettingsWidget->MainMenuRef = MainMenuRef;
        VideoSettingsWidget->OptionsMenuRef = this; // �������� �ɼ� ����?
    }

    // ���� ����� �ǿ��� �ɼ� ��� �ְ� 
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
        // ���� �޴����� ESC �� ����
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

        // �ΰ��ӿ����� PRPlayerController�� ToggleSettingsMenu() ó��
        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        if (PC)
        {
            if (APRPlayerController* PRPC = Cast<APRPlayerController>(PC))
            {
                PRPC->ToggleSettingsMenu();
                return FReply::Handled();
            }
        }

        // ��� �ʵ� �ƴϸ� ����
    }

    return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UOptionsMenuWidget::NativeConstruct()
{
    // ESC Ű �Է��� �ޱ� ���� ��Ŀ�� �����ϰ� ����
    SetIsFocusable(true);


}
