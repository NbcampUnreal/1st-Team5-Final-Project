// PayRockGames

#include "PayRock/UI/WidgetController/OverlayWidgetController.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"
#include "PayRock/Player/PRPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UPRAttributeSet* PRAttributeSet = Cast<UPRAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(PRAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(PRAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(PRAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(PRAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UPRAttributeSet* PRAttributeSet = Cast<UPRAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PRAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PRAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PRAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PRAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChanged);
	Cast<UPRAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTagsDelegate.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			if (!IsValid(MessageWidgetDataTable)) return;
			for (const FGameplayTag& Tag : AssetTags)
			{
				FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
			}
		}
	);

	if (APRPlayerState* PS = Cast<APRPlayerState>(PlayerState))
	{
		PS->OnDeathDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastDeath);
		PS->OnLevelChangeDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastLevelChange);
	}
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::BroadcastDeath() const
{
	OnDeath.Broadcast();
}

void UOverlayWidgetController::BroadcastLevelChange(int32 NewLevel) const
{
	OnLevelChange.Broadcast(NewLevel);
}
