#include "GA_VFX_Fireball.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"
#include "FireballProjectile.h"

UGA_VFX_Fireball::UGA_VFX_Fireball()
{
	SetAssetTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Event.Montage.Boss.Fire")));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Boss.State.Attacking"));
}

void UGA_VFX_Fireball::OnAuraEffectComplete()
{
	if (!Caster.IsValid())
	{
		FinishAbility();
		return;
	}

	DetectedTargets.Empty();
	for (auto Actor : Caster->GetDetectedActors())
	{
		if (Actor.IsValid())
		{
			DetectedTargets.Add(Actor);
		}
	}

	if (DetectedTargets.Num() == 0)
	{
		FinishAbility();
		return;
	}

	TotalFireballsToSpawn = DetectedTargets.Num() * 3;
	CurrentFireballIndex = 0;

	SpawnNextFireball();
}

void UGA_VFX_Fireball::SpawnNextFireball()
{
	if (!Caster.IsValid() || !FireballClass)
	{
		FinishAbility();
		return;
	}

	if (CurrentFireballIndex >= TotalFireballsToSpawn)
	{
		FinishAbility();
		return;
	}

	FVector Origin = Caster->GetActorLocation();
	FVector Forward = Caster->GetActorForwardVector();
	FVector Right = Caster->GetActorRightVector();

	const float ForwardOffset = FMath::FRandRange(MinForwardOffset, MaxForwardOffset);
	const float SideOffset = FMath::FRandRange(-SideOffsetRange, SideOffsetRange);
	const float ZOffset = VerticalOffset;

	FVector SpawnLocation = Origin + (Forward * ForwardOffset) + (Right * SideOffset);
	SpawnLocation.Z += ZOffset;

	FRotator SpawnRotation = FRotator::ZeroRotator;
	FActorSpawnParameters Params;
	Params.Owner = Caster.Get();
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AFireballProjectile* SpawnedFireball = GetWorld()->SpawnActor<AFireballProjectile>(
		FireballClass, SpawnLocation, SpawnRotation, Params);
	
	if (SpawnedFireball)
	{
		SpawnedFireball->InitializeEffectSource(this);
	}

	CurrentFireballIndex++;
	GetWorld()->GetTimerManager().SetTimer(
		FireballSequenceTimerHandle,
		this,
		&UGA_VFX_Fireball::SpawnNextFireball,
		0.3f,
		false
	);
}
