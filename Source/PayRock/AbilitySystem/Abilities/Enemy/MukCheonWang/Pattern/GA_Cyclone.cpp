#include "GA_Cyclone.h"
#include "CycloneActor.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"

UGA_Cyclone::UGA_Cyclone()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Event.Montage.Boss.Cyclone"));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Boss.State.Attacking"));
}

void UGA_Cyclone::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) return;

	AvatarActor = GetAvatarActorFromActorInfo();
	CurrentSpecHandle = Handle;
	CurrentActorInfo = const_cast<FGameplayAbilityActorInfo*>(ActorInfo);
	CurrentActivationInfo = ActivationInfo;
	
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&UGA_Cyclone::SpawnCycloneVDB,
		SpawnDelay,
		false
	);
}

void UGA_Cyclone::SpawnCycloneVDB()
{
	if (!AvatarActor || !CycloneClass) return;

	const FVector ForwardDir = AvatarActor->GetActorForwardVector();
	const FVector SpawnLoc = AvatarActor->GetActorLocation() + ForwardDir * ForwardOffset;
	const FRotator SpawnRot = AvatarActor->GetActorRotation();

	FActorSpawnParameters Params;
	Params.Owner = AvatarActor;
	Params.Instigator = AvatarActor->GetInstigator();

	ACycloneActor* Cyclone = GetWorld()->SpawnActor<ACycloneActor>(CycloneClass, SpawnLoc, SpawnRot, Params);
	if (!Cyclone) return;
	
	Cyclone->OnCycloneDestroyed = [this]()
	{
		OnCycloneEnded();
	};

	SpawnedCyclone = Cyclone;
}

void UGA_Cyclone::OnCycloneEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
