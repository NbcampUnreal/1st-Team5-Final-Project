#include "GA_VFX_StoneImpale.h"
#include "StoneSpikeActor.h"
#include "NiagaraFunctionLibrary.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"

UGA_VFX_StoneImpale::UGA_VFX_StoneImpale()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Event.Montage.Boss.Stone"));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Boss.State.Attacking"));
}

void UGA_VFX_StoneImpale::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AvatarActor = GetAvatarActorFromActorInfo();
	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(AvatarActor);
	if (!Boss || !StoneSpikeClass)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	CurrentSpecHandle = Handle;
	CurrentActorInfo = const_cast<FGameplayAbilityActorInfo*>(ActorInfo);
	CurrentActivationInfo = ActivationInfo;
	
	PlayAuraVFX(AvatarActor);
	
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			AuraDelayTimerHandle,
			this,
			&UGA_VFX_StoneImpale::SpawnStoneSpikesAfterAura,
			AuraDelayTime,
			false
		);
	}
}

void UGA_VFX_StoneImpale::SpawnStoneSpikesAfterAura()
{
	GetWorld()->GetTimerManager().ClearTimer(AuraDelayTimerHandle);
	
	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(AvatarActor);
	if (!Boss || !StoneSpikeClass)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	TArray<AActor*> Targets = Boss->GetDetectedActors();
	FActorSpawnParameters Params;
	Params.Owner = Boss;

	for (AActor* Target : Targets)
	{
		if (!Target) continue;

		FVector ToTarget = (Target->GetActorLocation() - Boss->GetActorLocation()).GetSafeNormal();
		FVector SpawnLoc = Boss->GetActorLocation() + ToTarget * SpawnDistance;
		FRotator SpawnRot = ToTarget.Rotation();

		AStoneSpikeActor* Spike = Boss->GetWorld()->SpawnActor<AStoneSpikeActor>(
			StoneSpikeClass, SpawnLoc, SpawnRot, Params);

		if (Spike)
		{
			Spike->SetInstigatorAbility(this);
			Spike->SetActorRotation(SpawnRot);
			Spike->SetTarget(Target);
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
