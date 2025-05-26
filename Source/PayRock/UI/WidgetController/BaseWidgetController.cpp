// PayRockGames

#include "BaseWidgetController.h"
#include "AbilitySystemComponent.h"

void UBaseWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

bool UBaseWidgetController::IsAbilitySystemValid() const
{
	return IsValid(AbilitySystemComponent) && IsValid(AttributeSet);
}

void UBaseWidgetController::BroadcastInitialValues()
{
}

void UBaseWidgetController::BindCallbacksToDependencies()
{
}
