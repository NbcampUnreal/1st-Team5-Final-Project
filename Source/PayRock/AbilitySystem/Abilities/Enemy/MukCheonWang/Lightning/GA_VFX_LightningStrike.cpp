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

void UGA_VFX_LightningStrike::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AvatarActor = GetAvatarActorFromActorInfo();
	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(AvatarActor);
	if (!Boss || !LightningClass || Boss->GetDetectedActors().Num() == 0)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	CurrentSpecHandle = Handle;
	CurrentActorInfo = const_cast<FGameplayAbilityActorInfo*>(ActorInfo);
	CurrentActivationInfo = ActivationInfo;

	Boss->Multicast_PlayAuraEffect(AuraEffect, FontlClass, AuraRate);

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			AuraDelayTimerHandle,
			this,
			&UGA_VFX_LightningStrike::SpawnLightningAfterAura,
			AuraDelayTime,
			false
		);
	}
}


void UGA_VFX_LightningStrike::SpawnLightningAfterAura()
{
	GetWorld()->GetTimerManager().ClearTimer(AuraDelayTimerHandle);

	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(AvatarActor);
	if (!Boss || !LightningClass) return;

	LightningTargets = Boss->GetDetectedActors();
	CurrentTargetIndex = 0;

	SpawnLightningForTarget();
}

void UGA_VFX_LightningStrike::SpawnLightningForTarget()
{
	if (!LightningTargets.IsValidIndex(CurrentTargetIndex))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	AActor* Target = LightningTargets[CurrentTargetIndex];
	if (!Target)
	{
		++CurrentTargetIndex;
		SpawnLightningForTarget();
		return;
	}

	const FVector PredictedLocation = Target->GetActorLocation() + Target->GetVelocity() * PredictDelay;
	CurrentLightningCenter = FVector(PredictedLocation.X, PredictedLocation.Y, Target->GetActorLocation().Z);
	NumLightningPerTarget = FMath::RandRange(2, 4);
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
		Lightning->SetInstigatorAbility(this);
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
