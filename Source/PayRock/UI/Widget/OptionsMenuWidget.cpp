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
    // 탭 스위처가 유효한지(널포인터) && TabIndex가 유효한지(ex) 4개의 위젯이면 0,1,2,3 가 유효하다.
    if (TabSwitcher && TabSwitcher->GetNumWidgets(/*언리얼제공함수*/) > TabIndex)
    {
        // 옵션 탭 바꾸기 
        TabSwitcher->/*UE제공*/SetActiveWidgetIndex(TabIndex);
    }
}

void UOptionsMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();


    //// Back 눌렀을때 돌아갈 수 있도록 들고 있기 
    //if (VideoSettingsWidget && MainMenuRef)
    //{
    //    VideoSettingsWidget->MainMenuRef = MainMenuRef;
    //}

    //if (AudioSettingsWidget && MainMenuRef)
    //{
    //    VideoSettingsWidget->MainMenuRef = MainMenuRef;
    //}



    // ; 함수들 연결
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

    // 기본 탭 설정 (비디오 탭)
    SwitchTab(0);


    //// Back 눌렀을때 돌아갈 수 있도록 들고 있게 주소 넘겨주기 
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

    // 하위 비디오 탭에서 들고 있게
    if (VideoSettingsWidget && MainMenuRef)
    {
        VideoSettingsWidget->MainMenuRef = MainMenuRef;
        VideoSettingsWidget->OptionsMenuRef = this; // 비디오에서 옵션 끄게?
    }

    // 하위 오디오 탭에서 옵션 들고 있게 
    if (AudioSettingsWidget && MainMenuRef)
    {
        AudioSettingsWidget->MainMenuRef = MainMenuRef;
        AudioSettingsWidget->OptionsMenuRef = this;

    }
    // ; todo; 
    // 하위  키설 정에서 들도록 설정

    // 하위 게임 플레이

}


FReply UOptionsMenuWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    if (InKeyEvent.GetKey() == EKeys::Escape)
    {
        // 메인 메뉴에서 ESC → 복귀
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

        // 인게임에서는 PRPlayerController가 ToggleSettingsMenu() 처리
        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        if (PC)
        {
            if (APRPlayerController* PRPC = Cast<APRPlayerController>(PC))
            {
                PRPC->ToggleSettingsMenu();
                return FReply::Handled();
            }
        }

        // 어느 쪽도 아니면 무시
    }

    return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UOptionsMenuWidget::NativeConstruct()
{
    // ESC 키 입력을 받기 위해 포커스 가능하게 설정
    SetIsFocusable(true);


}
