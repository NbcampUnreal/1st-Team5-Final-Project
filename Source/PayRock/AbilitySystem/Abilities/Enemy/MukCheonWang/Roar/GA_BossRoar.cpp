#include "GA_BossRoar.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/Character.h"

UGA_BossRoar::UGA_BossRoar()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Event.Montage.Boss.Roar"));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Boss.State.Roaring"));
	ActiveRoarTag = FGameplayTag::RequestGameplayTag("Boss.State.Roaring");
}

void UGA_BossRoar::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* Boss = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!Boss || !RoarMontage)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	CurrentSpecHandle = Handle;
	CurrentActorInfo = const_cast<FGameplayAbilityActorInfo*>(ActorInfo);
	CurrentActivationInfo = ActivationInfo;
	
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, RoarMontage, 1.0f);

	Task->OnCompleted.AddDynamic(this, &UGA_BossRoar::OnRoarMontageEnded);
	Task->OnInterrupted.AddDynamic(this, &UGA_BossRoar::OnRoarMontageCancelled);
	Task->OnCancelled.AddDynamic(this, &UGA_BossRoar::OnRoarMontageCancelled);
	Task->ReadyForActivation();
}

void UGA_BossRoar::OnRoarMontageEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_BossRoar::OnRoarMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
