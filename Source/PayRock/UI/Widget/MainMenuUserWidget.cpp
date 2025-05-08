// PayRockGames


//#include "UI/Widget/MainMenuUserWidget.h"
#include "MainMenuUserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuUserWidget::HandleStartGameClicked()
{
    // ���� ���� �̸��� ��Ȯ�ϰ� �����ϼ��� (��: "MainLevel")
    UGameplayStatics::OpenLevel(this, FName("MainLevel"));
}

void UMainMenuUserWidget::HandleQuitGameClicked()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);

}

void UMainMenuUserWidget::HandleOptionsClicked()
{
    // �ɼ� UI �Ǵ� ���� â ���� ���� (���� ����)

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
