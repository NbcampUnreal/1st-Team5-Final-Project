// PayRockGames

#include "BaseHUD.h"
#include "PayRock/UI/Widget/BaseUserWidget.h"
#include "PayRock/UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* ABaseHUD::GetInGameHUDWidgetController(const FWidgetControllerParams& WCParams)
{
	if (InGameHUDWidgetController == nullptr)
	{
		InGameHUDWidgetController = NewObject<UOverlayWidgetController>(this, InGameHUDWidgetControllerClass);
		InGameHUDWidgetController->SetWidgetControllerParams(WCParams);
		InGameHUDWidgetController->BindCallbacksToDependencies();
		return InGameHUDWidgetController;
	}
	return InGameHUDWidgetController;
}

void ABaseHUD::InitInGameHUD(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(InGameHUDWidgetClass, TEXT("InGameHUDWidgetClass uninitialized - BP_BaseHUD"));
	checkf(InGameHUDWidgetControllerClass, TEXT("InGameHUDWidgetControllerClass uninitialized - BP_BaseHUD"));

	// Create InGameHUDWidget
	InGameHUDWidget = CreateWidget<UBaseUserWidget>(GetWorld(), InGameHUDWidgetClass);

	// Construct and assign InGameHUDWidgetController to InGameHUDWidget
	const FWidgetControllerParams WCParams(PC, PS, ASC, AS);
	InGameHUDWidget->SetWidgetController(GetInGameHUDWidgetController(WCParams));

	// Broadcast initial values after InGameHUDWidgetController has been initialized
	InGameHUDWidgetController->BroadcastInitialValues();
	
	InGameHUDWidget->AddToViewport();
}