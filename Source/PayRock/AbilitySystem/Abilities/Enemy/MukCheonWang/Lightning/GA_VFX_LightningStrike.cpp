#include "GA_VFX_LightningStrike.h"
#include "LightningStrikeActor.h"
#include "NiagaraFunctionLibrary.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"
#include "Kismet/KismetMathLibrary.h"

UGA_VFX_LightningStrike::UGA_VFX_LightningStrike()
{
	SetAssetTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Event.Montage.Boss.Lightning")));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Boss.State.Attacking"));
}

void UGA_VFX_LightningStrike::OnAuraEffectComplete()
{
	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(AvatarActor);
	if (!Boss || !LightningClass || Boss->GetDetectedActors().Num() == 0)
	{
		FinishAbility();
		return;
	}

	LightningTargets = Boss->GetDetectedActors();
	CurrentTargetIndex = 0;

	SpawnLightningForTarget();
}

void UGA_VFX_LightningStrike::SpawnLightningForTarget()
{
	if (!LightningTargets.IsValidIndex(CurrentTargetIndex))
	{
		FinishAbility();
		return;
	}

	AActor* Target = LightningTargets[CurrentTargetIndex].Get();
	if (!Target)
	{
		++CurrentTargetIndex;
		SpawnLightningForTarget();
		return;
	}

	const FVector PredictedLocation = Target->GetActorLocation() + Target->GetVelocity() * PredictDelay;
	CurrentLightningCenter = FVector(PredictedLocation.X, PredictedLocation.Y, Target->GetActorLocation().Z);
	NumLightningPerTarget = FMath::RandRange(5, 10);
	CurrentLightningCount = 0;

	SpawnNextLightning();
}

void UGA_VFX_LightningStrike::SpawnNextLightning()
{
	if (CurrentLightningCount >= NumLightningPerTarget)
	{
		++CurrentTargetIndex;
		SpawnLightningForTarget();
		return;
	}

	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(AvatarActor);
	if (!Boss || !LightningClass) return;

	const FVector Offset2D = FVector(
		FMath::FRandRange(-200.f, 200.f),
		FMath::FRandRange(-200.f, 200.f),
		0.f
	);

	FVector SpawnLoc = CurrentLightningCenter + Offset2D;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Boss;

	ALightningStrikeActor* Lightning = Boss->GetWorld()->SpawnActor<ALightningStrikeActor>(
		LightningClass, SpawnLoc, FRotator::ZeroRotator, SpawnParams);

	if (Lightning)
	{
		Lightning->InitializeEffectSource(this);
	}

	++CurrentLightningCount;

	GetWorld()->GetTimerManager().SetTimer(
		LightningChainTimer,
		this,
		&UGA_VFX_LightningStrike::SpawnNextLightning,
		LightningSpawnDelay,
		false
	);
}
