// PayRockGames

#include "StatWidgetController.h"
#include "AbilitySystemComponent.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"
#include "PayRock/Character/PRCharacter.h"
#include "PayRock/Character/Blessing/BlessingComponent.h"

void UStatWidgetController::BroadcastInitialValues()
{
	if (!AbilitySystemComponent || !AttributeSet) return;
	
	InitializeAttributesArray();

	for (const FGameplayAttribute& Attribute : Attributes)
	{
		OnAttributeChangeDelegate.Broadcast(Attribute.AttributeName, Attribute.GetNumericValue(AttributeSet));
	}

	InitializeBlessingComponentRef();
	if (!BlessingComponent) return;
	
	BroadcastActiveBlessingChange(BlessingComponent->GetEquippedActiveBlessingData());
	BroadcastPassiveBlessingChange(BlessingComponent->GetEquippedPassiveBlessingData());
}

void UStatWidgetController::InitializeAttributesArray()
{
	if (!Attributes.IsEmpty()) return;
	
	UPRAttributeSet* PRAttributeSet = Cast<UPRAttributeSet>(AttributeSet);
	if (!PRAttributeSet) return;

	UAttributeSet::GetAttributesFromSetClass(UPRAttributeSet::StaticClass(), Attributes);

	// Exclude current health and mana from the list of attributes to bind to
	Attributes = Attributes.FilterByPredicate([](const FGameplayAttribute& Attribute)
	{
		return !(Attribute.AttributeName == FString("Health") || Attribute.AttributeName == FString("Mana"));
	});
}

void UStatWidgetController::InitializeBlessingComponentRef()
{
	if (APRCharacter* Character = Cast<APRCharacter>(AbilitySystemComponent->GetAvatarActor()))
	{
		BlessingComponent = Character->BlessingComponent;
	}
}

void UStatWidgetController::BindCallbacksToDependencies()
{
	if (!AbilitySystemComponent || !AttributeSet) return;
	
	for (const FGameplayAttribute& Attribute : Attributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(
			this, &UStatWidgetController::BroadcastAttributeChange);
	}

	if (!BlessingComponent) return;

	BlessingComponent->OnActiveBlessingChange.AddUniqueDynamic(
		this, &UStatWidgetController::BroadcastActiveBlessingChange);
	BlessingComponent->OnPassiveBlessingChange.AddUniqueDynamic(
		this, &UStatWidgetController::BroadcastPassiveBlessingChange);
}

void UStatWidgetController::BroadcastAttributeChange(const FOnAttributeChangeData& Data)
{
	OnAttributeChangeDelegate.Broadcast(Data.Attribute.AttributeName, Data.NewValue);
}

void UStatWidgetController::BroadcastActiveBlessingChange(UBlessingDataAsset* BlessingDataAsset)
{
	OnActiveBlessingChange.Broadcast(BlessingDataAsset);
}

void UStatWidgetController::BroadcastPassiveBlessingChange(UBlessingDataAsset* BlessingDataAsset)
{
	OnPassiveBlessingChange.Broadcast(BlessingDataAsset);
}
