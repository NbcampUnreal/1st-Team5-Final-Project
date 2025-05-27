#include "GA_SpawnHealingTrees.h"
#include "HealingTreeActor.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"

UGA_SpawnHealingTrees::UGA_SpawnHealingTrees()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Event.Montage.Boss.Wood"));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Boss.State.Attacking"));
}

void UGA_SpawnHealingTrees::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AvatarActor = GetAvatarActorFromActorInfo();
	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(AvatarActor);
	if (!Boss || !TreeClass)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	CurrentSpecHandle = Handle;
	CurrentActorInfo = const_cast<FGameplayAbilityActorInfo*>(ActorInfo);
	CurrentActivationInfo = ActivationInfo;
	
	if (AuraEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			AuraEffect,
			Boss->GetRootComponent(),
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);
	}

	if (AuraDecalClass)
	{
		FActorSpawnParameters AuraParams;
		AuraParams.Owner = Boss;

		Boss->GetWorld()->SpawnActor<AActor>(
			AuraDecalClass,
			Boss->GetActorLocation(),
			FRotator::ZeroRotator,
			AuraParams
		);
	}
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			AuraDelayTimerHandle,
			this,
			&UGA_SpawnHealingTrees::SpawnTreesAfterAura,
			AuraDelayTime,
			false
		);
	}
}

void UGA_SpawnHealingTrees::SpawnTreesAfterAura()
{
	GetWorld()->GetTimerManager().ClearTimer(AuraDelayTimerHandle);
	
	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(AvatarActor);
	if (!Boss || !TreeClass)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	int32 Count = FMath::RandRange(MinSpawnCount, MaxSpawnCount);

	for (int32 i = 0; i < Count; ++i)
	{
		FVector RandOffset = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(300.f, SpawnRadius);
		FVector SpawnLoc = Boss->GetActorLocation() + RandOffset;
		SpawnLoc.Z += 50.f;

		FActorSpawnParameters Params;
		Params.Owner = Boss;

		AHealingTreeActor* Tree = Boss->GetWorld()->SpawnActor<AHealingTreeActor>(
			TreeClass, SpawnLoc, FRotator::ZeroRotator, Params);

		if (Tree)
		{
			Tree->SetBoss(Boss);
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
