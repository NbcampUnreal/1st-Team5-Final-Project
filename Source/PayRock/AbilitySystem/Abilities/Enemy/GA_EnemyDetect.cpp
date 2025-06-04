// PayRockGames

#include "GA_EnemyDetect.h"
#include "PayRock/Enemy/EnemyCharacter.h"

UGA_EnemyDetect::UGA_EnemyDetect()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_EnemyDetect::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AEnemyCharacter* Character = Cast<AEnemyCharacter>(GetAvatarActorFromActorInfo());
	if (!DetectMontage || !Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	Character->Multicast_PlayDetectMontage(DetectMontage);
	
	FTimerHandle LocalTimerHandle;
	const float Duration = DetectMontage->GetPlayLength();
	
	TWeakObjectPtr<UGA_EnemyDetect> WeakThis(this);

	FTimerDelegate TimerDel;
	TimerDel.BindLambda([WeakThis]()
	{
		if (WeakThis.IsValid())
		{
			WeakThis->EndAbility(
				WeakThis->CurrentSpecHandle,
				WeakThis->CurrentActorInfo,
				WeakThis->CurrentActivationInfo,
				true,
				false
			);
		}
	});

	if (Character->GetWorld())
	{
		Character->GetWorld()->GetTimerManager().SetTimer(LocalTimerHandle, TimerDel, Duration, false);
	}
}

