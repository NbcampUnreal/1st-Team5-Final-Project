// PayRockGames


//#include "UI/Widget/MainMenuUserWidget.h"
#include "MainMenuUserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "PayRock/UI/Widget/OptionsMenuWidget.h" // UOptionsMenuWidget ���
#include "HelpWidgetMenu.h"

void UMainMenuUserWidget::HandleStartGameClicked()
{
    // ���� ���� �̸��� ��Ȯ�ϰ� �����ϼ��� (��: "MainLevel")
    // ���� ��ư ������ -> �κ� �� 
    UGameplayStatics::OpenLevel(this, FName("TempMainMenu"));
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
            //// ;�ɼ� ������ ������ �ִ� Ref ���θ޴� �Ѱ��ֱ�!!! ���� ����!
            //OptionsWidget->MainMenuRef = this;

            OptionsWidget->AddToViewport();
            //// ;�ɼ� ������ ������ �ִ� Ref ���θ޴� �Ѱ��ֱ�!!! ���� ����!
            //OptionsWidget->MainMenuRef = this;

            // Ŀ���� �Լ�
            OptionsWidget->InitOptionsMenu(this); //  �� ���� ���� ����


            APlayerController* PC = GetWorld()->GetFirstPlayerController();

            // ���콺�� Ŭ�� ���ص� �ٷ� esc�� �������� ���� 
            if (PC)
            {
                FInputModeUIOnly InputMode;
                InputMode.SetWidgetToFocus(OptionsWidget->TakeWidget()); //  �̰�!
                InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                PC->SetInputMode(InputMode);
                PC->bShowMouseCursor = true;
            }


            //OptionsWidget->AddToViewport();


            // �ڽ� ����ų� ��Ȱ��ȭ�� ���� ����
            this->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void UMainMenuUserWidget::HandleHelpClicked()
{
    if (HelpWidgetClass)
    {
        UHelpWidgetMenu* HelpWidget = CreateWidget<UHelpWidgetMenu>(GetWorld(), HelpWidgetClass);

        if (HelpWidget)
        {
            HelpWidget->MainMenuRef = this; //  ���� �޴� ���� ���� �ѱ��

            HelpWidget->AddToViewport();

            APlayerController* PC = GetWorld()->GetFirstPlayerController();
            if (PC)
            {
                FInputModeUIOnly InputMode;
                InputMode.SetWidgetToFocus(HelpWidget->TakeWidget());
                InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                PC->SetInputMode(InputMode);
                PC->bShowMouseCursor = true;
            }

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


    if (HelpButton)
    {
        HelpButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::HandleHelpClicked);
    }
}
