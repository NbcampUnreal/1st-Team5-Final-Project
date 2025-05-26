// PayRockGames


#include "GA_VFX_Fireball.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"


UGA_VFX_Fireball::UGA_VFX_Fireball()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Event.Montage.Boss.Fire"));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Attacking"));
}


void UGA_VFX_Fireball::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* Avatar = GetAvatarActorFromActorInfo();
	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(Avatar);
	TArray<AActor*> DetectedActors = Boss->GetDetectedActors();
	if (!Boss || !FireballClass || DetectedActors.Num() == 0)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}


	AActor* Target = DetectedActors[FMath::RandRange(0, DetectedActors.Num() - 1)];
	if (!Target) return;


	FVector SpawnLocation = Boss->GetActorLocation() + FVector(0, 0, 100);
	FRotator SpawnRotation = (Target->GetActorLocation() - SpawnLocation).Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Boss;

	AFireballProjectile* Fireball = Boss->GetWorld()->SpawnActor<AFireballProjectile>(FireballClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (Fireball)
	{
		Fireball->InitTarget(Target);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
