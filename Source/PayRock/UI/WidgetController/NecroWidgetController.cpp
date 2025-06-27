// PayRockGames


#include "NecroWidgetController.h"
#include "PayRock/PRGameplayTags.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "PayRock/Player/PRPlayerController.h"

void UNecroWidgetController::HandleRemoval()
{
	for (auto& Pair : CooldownUpdateTimers.Array())
	{
		if (GetWorld() && !GetWorld()->bIsTearingDown)
		{
			GetWorld()->GetTimerManager().ClearTimer(Pair.Value);	
		}
	}

	for (auto& Pair : CooldownDelegates.Array())
	{
		if (GetWorld() && !GetWorld()->bIsTearingDown)
		{
			Pair.Value.Clear();
		}
	}
}

void UNecroWidgetController::BroadcastInitialValues()
{
}

void UNecroWidgetController::BindCallbacksToDependencies()
{
	// Cooldown delegates
	AbilitySystemComponent->RegisterGameplayTagEvent(FPRGameplayTags::Get().Ability_Necro_Cooldown_Expel).AddUObject(
		this, &UNecroWidgetController::CooldownChanged);
	AbilitySystemComponent->RegisterGameplayTagEvent(FPRGameplayTags::Get().Ability_Necro_Cooldown_Slow).AddUObject(
		this, &UNecroWidgetController::CooldownChanged);
	AbilitySystemComponent->RegisterGameplayTagEvent(FPRGameplayTags::Get().Ability_Necro_Cooldown_Blind).AddUObject(
		this, &UNecroWidgetController::CooldownChanged);

	if (APRPlayerController* PC = Cast<APRPlayerController>(PlayerController))
	{
		PC->OnExtractionEnabled.AddUniqueDynamic(this, &UNecroWidgetController::BroadcastExtractionEnabled);
	}
}

void UNecroWidgetController::CooldownChanged(const FGameplayTag Tag, int32 TagCount)
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

void UNecroWidgetController::BroadcastCooldown(const FGameplayTag& Tag)
{
	float RemainingTime = 0.f;
	if (!IsValid(AbilitySystemComponent)) return;
	if (UPRAbilitySystemComponent* PRASC = Cast<UPRAbilitySystemComponent>(AbilitySystemComponent))
	{
		RemainingTime = PRASC->GetCooldownRemainingTimeForTag(Tag);
	}
	if (FOnCooldownChanged* Delegate = CooldownDelegates.Find(Tag))
	{
		if (Delegate->IsBound())
		{
			Delegate->Broadcast(RemainingTime);	
		}
	}
}

void UNecroWidgetController::BroadcastExtractionEnabled()
{
	OnNotificationRequest.Broadcast(ENotificationType::ExtractionEnabled);
}
