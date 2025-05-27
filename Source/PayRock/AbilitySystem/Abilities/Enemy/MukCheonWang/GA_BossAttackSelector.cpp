// PayRockGames

#include "GA_BossAttackSelector.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"

UGA_BossAttackSelector::UGA_BossAttackSelector()
{

	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Event.Montage.Boss.Selector"));
	
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Boss.State.Attacking"));
}

void UGA_BossAttackSelector::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* Instigator = GetAvatarActorFromActorInfo();
	if (!Instigator)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	float HealthRatio = GetBossHealthRatio();
	
	TArray<FWeightedAttackTag> ValidTags;
	for (const FWeightedAttackTag& Entry : WeightedAttackTags)
	{
		if (Entry.AttackTag == FGameplayTag::RequestGameplayTag("Event.Montage.Boss.Wood") && HealthRatio > 0.6f)
		{
			continue;
		}
		ValidTags.Add(Entry);
	}
	
	if (ValidTags.Num() == 0)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	float TotalWeight = 0.f;
	for (const auto& Entry : ValidTags)
	{
		TotalWeight += Entry.Weight;
	}

	float Roll = FMath::FRandRange(0.f, TotalWeight);
	float Accumulated = 0.f;

	for (const auto& Entry : ValidTags)
	{
		Accumulated += Entry.Weight;
		if (Roll <= Accumulated)
		{
			FGameplayEventData Payload;
			Payload.EventTag = Entry.AttackTag;
			Payload.Instigator = Instigator;
			Payload.Target = Instigator;

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Instigator, Entry.AttackTag, Payload);
			break;
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

float UGA_BossAttackSelector::GetBossHealthRatio() const
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (const UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Avatar))
	{
		const UPRAttributeSet* Attr = ASC->GetSet<UPRAttributeSet>(); 
		if (Attr && Attr->GetMaxHealth() > 0)
		{
			return Attr->GetHealth() / Attr->GetMaxHealth();
		}
	}
	return 1.f; 
}
