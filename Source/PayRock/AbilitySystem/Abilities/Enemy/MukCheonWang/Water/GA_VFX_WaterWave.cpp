// PayRockGames


#include "GA_VFX_WaterWave.h"


UGA_VFX_WaterWave::UGA_VFX_WaterWave()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Event.Montage.Boss.Water"));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Attacking"));
}

void UGA_VFX_WaterWave::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar || !WaveClass)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	FVector SpawnLoc = Avatar->GetActorLocation();
	FRotator SpawnRot = bWaveAlongX ? FRotator(0, 0, 0) : FRotator(0, 90, 0);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Avatar;

	AWaterWave* Wave = Avatar->GetWorld()->SpawnActor<AWaterWave>(
		WaveClass, SpawnLoc, SpawnRot, SpawnParams);

	if (Wave)
	{
		Wave->SetActorScale3D(WaveScale);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}