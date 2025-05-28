// PayRockGames


//#include "UI/Widget/MainMenuUserWidget.h"
#include "MainMenuUserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "PayRock/UI/Widget/OptionsMenuWidget.h" // UOptionsMenuWidget 헤더

void UMainMenuUserWidget::HandleStartGameClicked()
{
    // 메인 레벨 이름을 정확하게 설정하세요 (예: "MainLevel")
    // 시작 버튼 누를때 -> 로비 로 
    UGameplayStatics::OpenLevel(this, FName("SessionLevel"));
}

void UMainMenuUserWidget::HandleQuitGameClicked()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);

}

void UMainMenuUserWidget::HandleOptionsClicked()
{
    // 옵션 UI 또는 설정 창 열기 로직 (추후 구현)
    if (OptionsMenuWidgetClass) // TSubclassOf<UOptionsMenuWidget>
    {
        UOptionsMenuWidget* OptionsWidget = CreateWidget<UOptionsMenuWidget>(GetWorld(), OptionsMenuWidgetClass);



        if (OptionsWidget)
        {
            //// ;옵션 위젯이 가지고 있는 Ref 메인메뉴 넘겨주기!!! 순서 조심!
            //OptionsWidget->MainMenuRef = this;

            OptionsWidget->AddToViewport();
            //// ;옵션 위젯이 가지고 있는 Ref 메인메뉴 넘겨주기!!! 순서 조심!
            //OptionsWidget->MainMenuRef = this;

            // 커스텀 함수
            OptionsWidget->InitOptionsMenu(this); //  그 다음 참조 세팅


            APlayerController* PC = GetWorld()->GetFirstPlayerController();
            if (PC)
            {
                PC->SetInputMode(FInputModeUIOnly());
                PC->bShowMouseCursor = true;
            }


            //OptionsWidget->AddToViewport();


            // 자신 숨기거나 비활성화할 수도 있음
            this->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void UMainMenuUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();



    if (StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::HandleStartGameClicked);
    }

    if (QuitGameButton)
    {
        QuitGameButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::HandleQuitGameClicked);
    }

    if (OptionsButton)
    {
        OptionsButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::HandleOptionsClicked);
    }
}
