// PayRockGames

#include "StatWidgetController.h"
#include "AbilitySystemComponent.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"
#include "PayRock/Character/PRCharacter.h"
#include "PayRock/Character/Blessing/BlessingComponent.h"
#include "PayRock/Player/PRPlayerState.h"
#include "PayRock/UI/Manager/UIManager.h"

void UStatWidgetController::BroadcastInitialValues()
{
	if (!AbilitySystemComponent || !AttributeSet) return;
	
	for (const FGameplayAttribute& Attribute : Attributes)
	{
		OnAttributeChangeDelegate.Broadcast(Attribute.AttributeName, Attribute.GetNumericValue(AttributeSet));
	}

	if (PlayerController && PlayerController->IsLocalController() && PlayerController->GetPawn())
	{
		if (APRCharacter* Character = Cast<APRCharacter>(PlayerController->GetPawn()))
		{
			if (Character->BlessingComponent)
			{
				OnActiveBlessingInfoSet.Broadcast(Character->BlessingComponent->GetEquippedActiveBlessingData());
			}
		}
	}

	if (APRPlayerState* PS = Cast<APRPlayerState>(PlayerState))
	{
		BroadcastAccessorySkillChange(PS->GetCurrentAccessoryID());
		BroadcastWeaponSkillChange(PS->GetCurrentWeaponID());
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

	if (APRPlayerState* PS = Cast<APRPlayerState>(PlayerState))
	{
		PS->OnAccessorySkillChangedDelegate.AddUObject(this, &UStatWidgetController::BroadcastAccessorySkillChange);
		PS->OnWeaponSkillChangedDelegate.AddUObject(this, &UStatWidgetController::BroadcastWeaponSkillChange);
	}
}

void UStatWidgetController::BroadcastAttributeChange(const FOnAttributeChangeData& Data) const
{
	OnAttributeChangeDelegate.Broadcast(Data.Attribute.AttributeName, Data.NewValue);
}

void UStatWidgetController::BroadcastAccessorySkillChange(const FName& AccessoryID) const
{
	if (IsValid(PlayerController) && PlayerController->IsLocalController())
	{
		if (UUIManager* UIManager = PlayerController->GetGameInstance()->GetSubsystem<UUIManager>())
		{
			const FSkillData Data = UIManager->GetSkillData(AccessoryID);
			OnAccessorySkillChanged.Broadcast(Data);
		}
	}
}

void UStatWidgetController::BroadcastWeaponSkillChange(const FName& WeaponID) const
{
	if (IsValid(PlayerController) && PlayerController->IsLocalController())
	{
		if (UUIManager* UIManager = PlayerController->GetGameInstance()->GetSubsystem<UUIManager>())
		{
			const FSkillData Data = UIManager->GetSkillData(WeaponID);
			OnWeaponSkillChanged.Broadcast(Data);
		}
	}
}
