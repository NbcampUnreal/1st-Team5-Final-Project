// PayRockGames

#include "StatHUD.h"
#include "PayRock/UI/Widget/BaseUserWidget.h"
#include "PayRock/UI/WidgetController/StatWidgetController.h"

UStatWidgetController* AStatHUD::GetStatWidgetController(const FWidgetControllerParams& WCParams)
{
	if (StatWidgetController == nullptr)
	{
		StatWidgetController = NewObject<UStatWidgetController>(this, StatWidgetControllerClass);
		StatWidgetController->SetWidgetControllerParams(WCParams);
		StatWidgetController->BindCallbacksToDependencies();
		return StatWidgetController;
	}
	return StatWidgetController;
}

void AStatHUD::InitStatWidget(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	if (!StatWidgetClass || !StatWidgetControllerClass) return;

	// Create StatWidget
	StatWidget = CreateWidget<UBaseUserWidget>(GetWorld(), StatWidgetClass);

	// Construct and assign StatWidgetController to StatWidget
	const FWidgetControllerParams WCParams(PC, PS, ASC, AS);
	StatWidget->SetWidgetController(GetStatWidgetController(WCParams));

	// Broadcast initial values after StatWidgetController has been initialized
	StatWidgetController->BroadcastInitialValues();
	
	StatWidget->AddToViewport();
}