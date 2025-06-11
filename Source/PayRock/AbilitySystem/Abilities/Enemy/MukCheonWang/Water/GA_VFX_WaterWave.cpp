#include "GA_VFX_WaterWave.h"
#include "WaterWave.h"
#include "TimerManager.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"

UGA_VFX_WaterWave::UGA_VFX_WaterWave()
{
	SetAssetTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Event.Montage.Boss.Water")));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Boss.State.Attacking"));
}

void UGA_VFX_WaterWave::OnAuraEffectComplete()
{
	if (!AvatarActor.IsValid() || !WaveClass)
	{
		FinishAbility();
		return;
	}

	NumWavesToSpawn = 1;
	SpawnedWaveCount = 0;

	SpawnNextWave();
}

void UGA_VFX_WaterWave::SpawnNextWave()
{
	if (!AvatarActor.IsValid() || !WaveClass || !AvatarActor->HasAuthority())
	{
		return;
	}

	FVector SpawnLoc = AvatarActor->GetActorLocation();
	FRotator SpawnRot = AvatarActor->GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = AvatarActor.Get();

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
		FinishAbility();
	}
}
