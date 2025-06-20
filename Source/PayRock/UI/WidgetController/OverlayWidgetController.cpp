// PayRockGames

#include "PayRock/UI/WidgetController/OverlayWidgetController.h"
#include "PayRock/PRGameplayTags.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"
#include "PayRock/Character/PRCharacter.h"
#include "PayRock/Character/Blessing/BlessingComponent.h"
#include "PayRock/Player/PRPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	if (!AttributeSet)
	{
		UE_LOG(LogTemp, Error, TEXT("[OverlayWidgetController] AttributeSet is null!"));
		return;
	}

	const UPRAttributeSet* PRAttributeSet = Cast<UPRAttributeSet>(AttributeSet);
	if (!PRAttributeSet)
	{
		UE_LOG(LogTemp, Error, TEXT("[OverlayWidgetController] Failed to cast AttributeSet to UPRAttributeSet!"));
		return;
	}

	OnHealthChanged.Broadcast(PRAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(PRAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(PRAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(PRAttributeSet->GetMaxMana());

	if (PlayerController && PlayerController->IsLocalController() && PlayerController->GetPawn())
	{
		if (APRCharacter* Character = Cast<APRCharacter>(PlayerController->GetPawn()))
		{
			if (Character->BlessingComponent)
			{
				OnActiveBlessingChanged.Broadcast(Character->BlessingComponent->LocalEquippedActiveBlessingIcon);
			}
		}
	}
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	if (!AttributeSet)
	{
		UE_LOG(LogTemp, Error, TEXT("[OverlayWidgetController] AttributeSet is null in BindCallbacksToDependencies!"));
		return;
	}

	const UPRAttributeSet* PRAttributeSet = Cast<UPRAttributeSet>(AttributeSet);

	if (!PRAttributeSet)
	{
		UE_LOG(LogTemp, Error, TEXT("[OverlayWidgetController] Failed to cast AttributeSet in BindCallbacksToDependencies!"));
		return;
	}

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PRAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PRAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PRAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PRAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChanged);

	// Cooldown delegates
	AbilitySystemComponent->RegisterGameplayTagEvent(FPRGameplayTags::Get().Ability_Blessing_Cooldown).AddUObject(
		this, &UOverlayWidgetController::CooldownChanged);
	AbilitySystemComponent->RegisterGameplayTagEvent(FPRGameplayTags::Get().Ability_AccessorySkill_Cooldown).AddUObject(
		this, &UOverlayWidgetController::CooldownChanged);
	AbilitySystemComponent->RegisterGameplayTagEvent(FPRGameplayTags::Get().Ability_WeaponSkill_Cooldown).AddUObject(
		this, &UOverlayWidgetController::CooldownChanged);

	// Weapon/Accessory skill changed
	

	if (APRPlayerState* PS = Cast<APRPlayerState>(PlayerState))
	{
		PS->OnDeathDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastDeath);
		PS->OnExtractionDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastExtraction);
		PS->OnAccessorySkillChangedDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAccessorySkillChange);
		PS->OnWeaponSkillChangedDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastWeaponSkillChange);
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

void UOverlayWidgetController::BroadcastExtraction() const
{
	OnExtraction.Broadcast();
}

void UOverlayWidgetController::BroadcastAccessorySkillChange(const FSkillData& SkillData) const
{
	OnAccessorySkillChanged.Broadcast(SkillData);
}

void UOverlayWidgetController::BroadcastWeaponSkillChange(const FSkillData& SkillData) const
{
	OnWeaponSkillChanged.Broadcast(SkillData);
}

void UOverlayWidgetController::CooldownChanged(const FGameplayTag Tag, int32 TagCount)
{
	if (!IsValid(AbilitySystemComponent)) return;
	if (!GetWorld() || GetWorld()->bIsTearingDown) return;
	
	if (TagCount > 0)
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([this, Tag]()
		{
			BroadcastCooldown(Tag);
		});

		FTimerHandle& TimerHandle = CooldownUpdateTimers.FindOrAdd(Tag);
			
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			TimerDelegate,
			0.1f,
			true
		);
	}
	else
	{
		if (CooldownUpdateTimers.Contains(Tag))
		{
			GetWorld()->GetTimerManager().ClearTimer(CooldownUpdateTimers[Tag]);
			CooldownUpdateTimers.Remove(Tag);

			if (FOnCooldownChanged* Delegate = CooldownDelegates.Find(Tag))
			{
				Delegate->Broadcast(0.f);	
			}
		}
	}
}

void UOverlayWidgetController::BroadcastCooldown(const FGameplayTag& Tag)
{
	float RemainingTime = 0.f;
	if (UPRAbilitySystemComponent* PRASC = Cast<UPRAbilitySystemComponent>(AbilitySystemComponent))
	{
		RemainingTime = PRASC->GetCooldownRemainingTimeForTag(Tag);
	}
	if (FOnCooldownChanged* Delegate = CooldownDelegates.Find(Tag))
	{
		Delegate->Broadcast(RemainingTime);
	}
}
