// PayRockGames


//#include "UI/Widget/MainMenuUserWidget.h"
#include "MainMenuUserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "PayRock/UI/Widget/OptionsMenuWidget.h" // UOptionsMenuWidget ���

void UMainMenuUserWidget::HandleStartGameClicked()
{
    // ���� ���� �̸��� ��Ȯ�ϰ� �����ϼ��� (��: "MainLevel")
    UGameplayStatics::OpenLevel(this, FName("Lobby"));
}

void UMainMenuUserWidget::HandleQuitGameClicked()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);

}

void UMainMenuUserWidget::HandleOptionsClicked()
{
    // �ɼ� UI �Ǵ� ���� â ���� ���� (���� ����)
    if (OptionsMenuWidgetClass) // TSubclassOf<UOptionsMenuWidget>
    {
        UOptionsMenuWidget* OptionsWidget = CreateWidget<UOptionsMenuWidget>(GetWorld(), OptionsMenuWidgetClass);
        if (OptionsWidget)
        {
            OptionsWidget->AddToViewport();
            APlayerController* PC = GetWorld()->GetFirstPlayerController();
            if (PC)
            {
                PC->SetInputMode(FInputModeUIOnly());
                PC->bShowMouseCursor = true;
            }

            // �ڽ� ����ų� ��Ȱ��ȭ�� ���� ����
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
