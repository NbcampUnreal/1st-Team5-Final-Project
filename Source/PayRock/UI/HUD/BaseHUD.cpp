// PayRockGames

#include "BaseHUD.h"
#include "PayRock/UI/Widget/BaseUserWidget.h"
#include "PayRock/UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* ABaseHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

void ABaseHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass uninitialized - BP_BaseHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass uninitialized - BP_BaseHUD"));

	// Create OverlayWidget
	OverlayWidget = CreateWidget<UBaseUserWidget>(GetWorld(), OverlayWidgetClass);

	// Construct and assign OverlayWidgetController to OverlayWidget
	const FWidgetControllerParams WCParams(PC, PS, ASC, AS);
	OverlayWidget->SetWidgetController(GetOverlayWidgetController(WCParams));

	// Broadcast initial values after OverlayWidgetController has been initialized
	OverlayWidgetController->BroadcastInitialValues();
	
	OverlayWidget->AddToViewport();
}