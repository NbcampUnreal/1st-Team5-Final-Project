// PayRockGames

#include "PRAbilitySystemComponent.h"
#include "Abilities/BaseGameplayAbility.h"
#include "PayRock/PRGameplayTags.h"

void UPRAbilitySystemComponent::OnAbilityActorInfoInitialized()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UPRAbilitySystemComponent::ClientOnEffectApplied);
}

void UPRAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		AddAbility(AbilityClass, false);
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UPRAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
	{
		AddAbility(AbilityClass, true);
	} 
}

FGameplayAbilitySpecHandle UPRAbilitySystemComponent::AddAbility(
	TSubclassOf<UGameplayAbility> AbilityClass, bool bIsPassive, int32 AbilityLevel)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, AbilityLevel);
	if (const UBaseGameplayAbility* BaseAbility = Cast<UBaseGameplayAbility>(AbilitySpec.Ability))
	{
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(BaseAbility->StartupInputTag);
	}
	//AbilitySpec.GetDynamicSpecSourceTags().AddTag(FPRGameplayTags::Get().Abilities_Status_Equipped);
	return bIsPassive ? GiveAbilityAndActivateOnce(AbilitySpec) : GiveAbility(AbilitySpec);
}

void UPRAbilitySystemComponent::RemoveAbility(const FGameplayAbilitySpecHandle& AbilitySpecHandle)
{
	ClearAbility(AbilitySpecHandle);
}

void UPRAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			/*if (AbilitySpec.IsActive())
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle,
					AbilitySpec.Ability->GetCurrentActivationInfo().GetActivationPredictionKey());
			}*/
		}
	}
}

void UPRAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UPRAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag) /*&& AbilitySpec.IsActive()*/)
		{
			AbilitySpecInputReleased(AbilitySpec);
			/*InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle,
					AbilitySpec.Ability->GetCurrentActivationInfo().GetActivationPredictionKey());*/
		}
	}
}

void UPRAbilitySystemComponent::ClientOnEffectApplied_Implementation(
	UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec,
	FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTagsDelegate.Broadcast(TagContainer);
}
