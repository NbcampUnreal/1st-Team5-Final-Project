// PayRockGames

#include "GA_BossAttackSelector.h"
#include "AbilitySystemBlueprintLibrary.h"

UGA_BossAttackSelector::UGA_BossAttackSelector()
{
	SetAssetTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Event.Montage.Boss.Selector")));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Attacking"));
}

void UGA_BossAttackSelector::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (AttackTags.Num() == 0)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	int32 Index = FMath::RandRange(0, AttackTags.Num() - 1);
	FGameplayTag ChosenTag = AttackTags[Index];

	AActor* InstigatorActor = const_cast<AActor*>(GetAvatarActorFromActorInfo());

	FGameplayEventData Payload;
	Payload.EventTag = ChosenTag;
	Payload.Instigator = InstigatorActor;
	Payload.Target = InstigatorActor;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(InstigatorActor, ChosenTag, Payload);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
