#include "GA_VFX_WaterWave.h"
#include "WaterWave.h"
#include "TimerManager.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"

UGA_VFX_WaterWave::UGA_VFX_WaterWave()
{
	SetAssetTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Event.Montage.Boss.Water")));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Boss.State.Attacking"));
}

void UGA_VFX_WaterWave::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor || !WaveClass)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	CurrentSpecHandle = Handle;
	CurrentActorInfo = const_cast<FGameplayAbilityActorInfo*>(ActorInfo);
	CurrentActivationInfo = ActivationInfo;
	
	if (AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(GetAvatarActorFromActorInfo()))
	{
		Boss->Multicast_PlayAuraEffect(AuraEffect, FontlClass, AuraRate);
	}


	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			AuraDelayTimerHandle,
			this,
			&UGA_VFX_WaterWave::SpawnWaterWaveAfterAura,
			AuraDelayTime,
			false
		);
	}
}

void UGA_VFX_WaterWave::SpawnWaterWaveAfterAura()
{
	GetWorld()->GetTimerManager().ClearTimer(AuraDelayTimerHandle);

	if (!AvatarActor || !WaveClass)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	NumWavesToSpawn = FMath::RandRange(1, 2);
	SpawnedWaveCount = 0;

	SpawnNextWave();
}

void UGA_VFX_WaterWave::SpawnNextWave()
{
	if (!AvatarActor || !WaveClass || !AvatarActor->HasAuthority())
	{
		return;
	}

	FVector SpawnLoc = AvatarActor->GetActorLocation();
	FRotator SpawnRot = AvatarActor->GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = AvatarActor;

	AWaterWave* Wave = GetWorld()->SpawnActor<AWaterWave>(WaveClass, SpawnLoc, SpawnRot, SpawnParams);
	if (Wave)
	{
		Wave->SetActorScale3D(WaveScale);
		Wave->SetReplicates(true);
	}

	SpawnedWaveCount++;

	if (SpawnedWaveCount < NumWavesToSpawn)
	{
		GetWorld()->GetTimerManager().SetTimer(
			WaveChainTimer,
			this,
			&UGA_VFX_WaterWave::SpawnNextWave,
			WaveSpawnDelay,
			false
		);
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}
