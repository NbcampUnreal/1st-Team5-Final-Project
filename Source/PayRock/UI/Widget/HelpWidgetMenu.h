// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuUserWidget.h"
#include "PayRock/UI/Widget/BaseUserWidget.h"
#include "HelpWidgetMenu.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UHelpWidgetMenu : public UBaseUserWidget
{
	GENERATED_BODY()

public:

    /** MainMenu ÂüÁ¶ */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Reference")
    UMainMenuUserWidget* MainMenuRef;

protected:
    virtual void NativeConstruct() override;
    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
