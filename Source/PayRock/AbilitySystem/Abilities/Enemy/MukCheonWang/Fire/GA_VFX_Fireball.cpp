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

void UGA_VFX_Fireball::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Caster = Cast<AMukCheonWangCharacter>(GetAvatarActorFromActorInfo());
	if (!Caster.IsValid()) return;

	PlayAuraVFX(Caster.Get());

	
	GetWorld()->GetTimerManager().SetTimer(
		AuraDelayTimerHandle, this, &UGA_VFX_Fireball::StartFireballSequence, AuraDelayTime, false);
}


void UGA_VFX_Fireball::StartFireballSequence()
{
	if (!Caster.IsValid()) return;

	DetectedTargets.Empty();
	for (AActor* Actor : Caster->GetDetectedActors())
	{
		if (IsValid(Actor)) DetectedTargets.Add(Actor);
	}

	if (DetectedTargets.Num() == 0)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
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
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	if (CurrentFireballIndex >= TotalFireballsToSpawn)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
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

	GetWorld()->SpawnActor<AFireballProjectile>(FireballClass, SpawnLocation, SpawnRotation, Params);

	
	CurrentFireballIndex++;
	GetWorld()->GetTimerManager().SetTimer(
		FireballSequenceTimerHandle,
		this,
		&UGA_VFX_Fireball::SpawnNextFireball,
		0.3f,
		false
	);
}
