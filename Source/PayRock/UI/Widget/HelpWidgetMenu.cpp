// PayRockGames


#include "PayRock/UI/Widget/HelpWidgetMenu.h"

void UHelpWidgetMenu::NativeConstruct()
{
	Super::NativeConstruct();
	SetIsFocusable(true); // ESC 키 입력 받기 위해 필요
}

FReply UHelpWidgetMenu::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    if (InKeyEvent.GetKey() == EKeys::Escape)
    {
        if (MainMenuRef)
        {
            MainMenuRef->SetVisibility(ESlateVisibility::Visible);

            APlayerController* PC = GetWorld()->GetFirstPlayerController();
            if (PC)
            {
                FInputModeUIOnly InputMode;
                InputMode.SetWidgetToFocus(MainMenuRef->TakeWidget());
                InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                PC->SetInputMode(InputMode);
                PC->bShowMouseCursor = true;
            }
        }

        RemoveFromParent();
        return FReply::Handled();
    }

    return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}
