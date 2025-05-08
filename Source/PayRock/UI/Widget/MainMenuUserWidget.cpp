// PayRockGames


//#include "UI/Widget/MainMenuUserWidget.h"
#include "MainMenuUserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuUserWidget::HandleStartGameClicked()
{
    // 메인 레벨 이름을 정확하게 설정하세요 (예: "MainLevel")
    UGameplayStatics::OpenLevel(this, FName("MainLevel"));
}

void UMainMenuUserWidget::HandleQuitGameClicked()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);

}

void UMainMenuUserWidget::HandleOptionsClicked()
{
    // 옵션 UI 또는 설정 창 열기 로직 (추후 구현)

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
