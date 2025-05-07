// PayRockGames

#include "EffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AEffectActor::AEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AEffectActor::ApplyEffectToTarget(AActor* TargetActor, const FGameplayEffectInfo& EffectInfo)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC) return;
	
	check(EffectInfo.GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(
		EffectInfo.GameplayEffectClass, EffectInfo.EffectLevel, ContextHandle);
	const FActiveGameplayEffectHandle EffectHandle =
		TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	
	const bool bIsInfinite = SpecHandle.Data->Def->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite)
	{
		ActiveEffectHandleInfoMap.Add(EffectHandle, EffectInfo);
	}
}

bool AEffectActor::RemoveEffectFromTarget(AActor* TargetActor,
	const FActiveGameplayEffectHandle& EffectHandle, const FGameplayEffectInfo& EffectInfo)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (IsValid(TargetASC) && TargetASC == EffectHandle.GetOwningAbilitySystemComponent())
	{
		TargetASC->RemoveActiveGameplayEffect(EffectHandle, EffectInfo.NumStacksToRemove);
		return true;
	}
	return false;
}

void AEffectActor::OnOverlap(AActor* TargetActor)
{
	for (const FGameplayEffectInfo& EffectInfo : GameplayEffects)
	{
		if (EffectInfo.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, EffectInfo);
		}
	}
}

void AEffectActor::OnEndOverlap(AActor* TargetActor)
{
	for (const FGameplayEffectInfo& EffectInfo : GameplayEffects)
	{
		if (EffectInfo.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, EffectInfo);
		}
	}

	TArray<FActiveGameplayEffectHandle> HandlesToRemove;
	for (const auto& HandlePair : ActiveEffectHandleInfoMap)
	{
		if (HandlePair.Value.EffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			if (RemoveEffectFromTarget(TargetActor, HandlePair.Key, HandlePair.Value))
			{
				HandlesToRemove.Add(HandlePair.Key);	
			}
		}
	}
	for (auto& Handle : HandlesToRemove)
	{
		ActiveEffectHandleInfoMap.Remove(Handle);
	}
	if (bDestroyOnEffectRemoval && ActiveEffectHandleInfoMap.Num() == 0)
	{
		Destroy();
	}
}