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
	UFUNCTION(BlueprintCallable)
	FGameplayAbilitySpecHandle AddAbility(TSubclassOf<UGameplayAbility> AbilityClass, bool bIsPassive = false, int32 AbilityLevel = 1);
	UFUNCTION(BlueprintCallable)
	void RemoveAbility(const FGameplayAbilitySpecHandle& AbilitySpecHandle);
	
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	float GetCooldownRemainingTimeForTag(const FGameplayTag CooldownTag) const;

protected:
	UFUNCTION(Client, Reliable)
	void ClientOnEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec,
		FActiveGameplayEffectHandle ActiveEffectHandle);

public:
	FEffectAssetTagsDelegate EffectAssetTagsDelegate;
	FAbilitiesGivenDelegate AbilitiesGivenDelegate;

	FHitResult StoredHitResult;
	FVector StoredHitDirection;

	bool bStartupAbilitiesGiven = false;
};
