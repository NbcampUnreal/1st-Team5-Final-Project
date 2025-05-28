// PayRockGames

#include "StatWidgetController.h"
#include "AbilitySystemComponent.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"
#include "PayRock/Character/PRCharacter.h"

void UStatWidgetController::BroadcastInitialValues()
{
	if (!AbilitySystemComponent || !AttributeSet) return;
	
	for (const FGameplayAttribute& Attribute : Attributes)
	{
		OnAttributeChangeDelegate.Broadcast(Attribute.AttributeName, Attribute.GetNumericValue(AttributeSet));
	}
}

void UStatWidgetController::InitializeAttributesArray()
{
	if (Attributes.IsEmpty())
	{
		UPRAttributeSet* PRAttributeSet = Cast<UPRAttributeSet>(AttributeSet);
		if (!PRAttributeSet) return;

		AbilitySystemComponent->GetAllAttributes(Attributes);

		// Exclude irrelevant attributes from the list of attributes to bind to
		Attributes = Attributes.FilterByPredicate([](const FGameplayAttribute& Attribute)
		{
			return !(Attribute.AttributeName == FString("Health")
				|| Attribute.AttributeName == FString("Mana")
				|| Attribute.AttributeName == FString("IncomingDamage")
				|| Attribute.AttributeName == FString("HealthRatio")
				|| Attribute.AttributeName == FString("ManaRatio"));
		});
	}

	if (AttributeNames.IsEmpty())
	{
		for (const auto& Attribute : Attributes)
		{
			AttributeNames.Add(Attribute.AttributeName);
		}
	}
}

void UStatWidgetController::BindCallbacksToDependencies()
{
	if (!AbilitySystemComponent || !AttributeSet) return;
	InitializeAttributesArray();
	for (const FGameplayAttribute& Attribute : Attributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(
			this, &UStatWidgetController::BroadcastAttributeChange);
	}
}

void UStatWidgetController::BroadcastAttributeChange(const FOnAttributeChangeData& Data)
{
	OnAttributeChangeDelegate.Broadcast(Data.Attribute.AttributeName, Data.NewValue);
}
