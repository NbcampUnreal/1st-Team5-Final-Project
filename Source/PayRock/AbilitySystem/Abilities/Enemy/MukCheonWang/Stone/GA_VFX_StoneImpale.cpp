#include "GA_VFX_StoneImpale.h"
#include "StoneSpikeActor.h"
#include "NiagaraFunctionLibrary.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"

UGA_VFX_StoneImpale::UGA_VFX_StoneImpale()
{
	SetAssetTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Event.Montage.Boss.Stone")));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Boss.State.Attacking"));
}

void UGA_VFX_StoneImpale::OnAuraEffectComplete()
{
	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(AvatarActor);
	if (!Boss || !StoneSpikeClass)
	{
		FinishAbility();
		return;
	}

	SpikeTargets = Boss->GetDetectedActors();
	CurrentTargetIndex = 0;

	if (SpikeTargets.Num() == 0)
	{
		FinishAbility();
		return;
	}

	SpawnSingleSpike();
}

void UGA_VFX_StoneImpale::SpawnSingleSpike()
{
	if (!SpikeTargets.IsValidIndex(CurrentTargetIndex))
	{
		FinishAbility();
		return;
	}

	AActor* Target = SpikeTargets[CurrentTargetIndex++].Get();
	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(AvatarActor);
	if (!Boss || !Target || !StoneSpikeClass) return;

	FVector ToTarget = (Target->GetActorLocation() - Boss->GetActorLocation()).GetSafeNormal();
	FVector SpawnLoc = Boss->GetActorLocation() + ToTarget * SpawnDistance;
	FRotator SpawnRot = ToTarget.Rotation();

	FActorSpawnParameters Params;
	Params.Owner = Boss;

	AStoneSpikeActor* Spike = Boss->GetWorld()->SpawnActor<AStoneSpikeActor>(StoneSpikeClass, SpawnLoc, SpawnRot, Params);
	if (Spike)
	{
		Spike->InitializeEffectSource(this);
		Spike->SetActorRotation(SpawnRot);
		Spike->SetTarget(Target);
	}

	Boss->GetWorld()->GetTimerManager().SetTimer(
		SpikeSpawnTimerHandle,
		this,
		&UGA_VFX_StoneImpale::SpawnSingleSpike,
		0.4f,
		false
	);
}
