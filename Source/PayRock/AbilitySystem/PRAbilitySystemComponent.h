// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PRAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagsDelegate, const FGameplayTagContainer /*AssetTags*/)
DECLARE_MULTICAST_DELEGATE(FAbilitiesGivenDelegate);

UCLASS()
class PAYROCK_API UPRAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void OnAbilityActorInfoInitialized();
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);

protected:
	void OnEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec,
		FActiveGameplayEffectHandle ActiveEffectHandle);

public:
	FEffectAssetTagsDelegate EffectAssetTagsDelegate;
	FAbilitiesGivenDelegate AbilitiesGivenDelegate;

	bool bStartupAbilitiesGiven = false;
};
