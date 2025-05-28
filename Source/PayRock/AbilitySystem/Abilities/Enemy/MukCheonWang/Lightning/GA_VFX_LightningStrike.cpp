#include "GA_VFX_LightningStrike.h"
#include "LightningStrikeActor.h"
#include "NiagaraFunctionLibrary.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"
#include "Kismet/KismetMathLibrary.h"

UGA_VFX_LightningStrike::UGA_VFX_LightningStrike()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Event.Montage.Boss.Lightning"));
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

	
	PlayAuraVFX(AvatarActor);


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
	if (!Boss || !LightningClass || Boss->GetDetectedActors().Num() == 0)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	AActor* Target = Boss->GetDetectedActors()[FMath::RandRange(0, Boss->GetDetectedActors().Num() - 1)];
	if (!Target)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	const FVector PredictedLocation = Target->GetActorLocation() + Target->GetVelocity() * PredictDelay;
	LightningCenter = FVector(PredictedLocation.X, PredictedLocation.Y, Target->GetActorLocation().Z);

	NumLightningToSpawn = FMath::RandRange(2, 4);
	SpawnedLightningCount = 0;

	SpawnNextLightning();
}

void UGA_VFX_LightningStrike::SpawnNextLightning()
{
	if (SpawnedLightningCount >= NumLightningToSpawn)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(AvatarActor);
	if (!Boss || !LightningClass) return;
	
	const FVector Offset2D = FVector(
		FMath::FRandRange(-200.f, 200.f),
		FMath::FRandRange(-200.f, 200.f),
		0.f
	);

	FVector SpawnLoc = LightningCenter + Offset2D;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Boss;

	ALightningStrikeActor* Lightning = Boss->GetWorld()->SpawnActor<ALightningStrikeActor>(
		LightningClass, SpawnLoc, FRotator::ZeroRotator, SpawnParams);

	if (Lightning)
	{
		Lightning->SetInstigatorAbility(this);
	}

	SpawnedLightningCount++;

	GetWorld()->GetTimerManager().SetTimer(
		LightningChainTimer,
		this,
		&UGA_VFX_LightningStrike::SpawnNextLightning,
		LightningSpawnDelay,
		false
	);
}
