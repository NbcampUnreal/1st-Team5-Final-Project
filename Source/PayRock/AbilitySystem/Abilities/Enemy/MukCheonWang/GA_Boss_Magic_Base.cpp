// PayRockGames
#include "GA_Boss_Magic_Base.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"

UGA_Boss_Magic_Base::UGA_Boss_Magic_Base()
{

}

void UGA_Boss_Magic_Base::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AvatarActor = GetAvatarActorFromActorInfo();
	Caster = Cast<AMukCheonWangCharacter>(AvatarActor.Get());

	if (!Caster.IsValid())
	{
		FinishAbility();
		return;
	}

	CurrentSpecHandle = Handle;
	CurrentActorInfo = const_cast<FGameplayAbilityActorInfo*>(ActorInfo);
	CurrentActivationInfo = ActivationInfo;

	Caster->Multicast_PlayAuraEffect(AuraEffect, FontlClass, AuraRate);

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			AuraDelayTimerHandle,
			this,
			&UGA_Boss_Magic_Base::OnAuraEffectComplete,
			AuraDelayTime,
			false
		);
	}
}

void UGA_Boss_Magic_Base::OnAuraEffectComplete()
{
}

void UGA_Boss_Magic_Base::FinishAbility()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
