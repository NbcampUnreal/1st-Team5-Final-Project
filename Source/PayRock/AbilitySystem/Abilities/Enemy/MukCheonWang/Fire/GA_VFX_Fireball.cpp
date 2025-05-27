// PayRockGames


#include "GA_VFX_Fireball.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"


UGA_VFX_Fireball::UGA_VFX_Fireball()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Event.Montage.Boss.Fire"));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Boss.State.Attacking"));
}


void UGA_VFX_Fireball::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	PlayAuraVFX(AvatarActor);

	
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			AuraDelayTimerHandle,
			this,
			&UGA_VFX_Fireball::SpawnFireballAfterAura,
			AuraDelayTime,  
			false
		);
	}
}

void UGA_VFX_Fireball::SpawnFireballAfterAura()
{
	GetWorld()->GetTimerManager().ClearTimer(AuraDelayTimerHandle);
	
	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(AvatarActor);
	if (!Boss || !FireballClass) return;

	TArray<AActor*> DetectedActors = Boss->GetDetectedActors();
	if (DetectedActors.Num() == 0)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	AActor* Target = DetectedActors[FMath::RandRange(0, DetectedActors.Num() - 1)];
	if (!Target) return;

	FVector SpawnLocation = Boss->GetActorLocation() + FVector(0, 0, 100);
	FRotator SpawnRotation = (Target->GetActorLocation() - SpawnLocation).Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Boss;

	AFireballProjectile* Fireball = Boss->GetWorld()->SpawnActor<AFireballProjectile>(
		FireballClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (Fireball)
	{
		Fireball->InitTarget(Target);
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
