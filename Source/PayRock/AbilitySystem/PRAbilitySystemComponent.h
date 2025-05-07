// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PRAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagsDelegate, const FGameplayTagContainer /*AssetTags*/)

UCLASS()
class PAYROCK_API UPRAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void OnAbilityActorInfoInitialized();

	FEffectAssetTagsDelegate EffectAssetTagsDelegate;
	
protected:
	
	void OnEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec,
		FActiveGameplayEffectHandle ActiveEffectHandle);
	
};
