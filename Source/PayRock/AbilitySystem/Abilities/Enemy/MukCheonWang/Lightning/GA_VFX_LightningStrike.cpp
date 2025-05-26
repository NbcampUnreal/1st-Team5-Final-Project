#include "GA_VFX_LightningStrike.h"
#include "LightningStrikeActor.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"
#include "Kismet/KismetMathLibrary.h"

UGA_VFX_LightningStrike::UGA_VFX_LightningStrike()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Event.Montage.Boss.Lightning"));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Attacking"));
}

void UGA_VFX_LightningStrike::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* Avatar = GetAvatarActorFromActorInfo();
	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(Avatar);
	if (!Boss || !LightningClass || Boss->GetDetectedActors().Num() == 0)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	
	AActor* Target = Boss->GetDetectedActors()[FMath::RandRange(0, Boss->GetDetectedActors().Num() - 1)];
	if (!Target) return;

	
	FVector PredictedLoc = Target->GetActorLocation() + Target->GetVelocity() * PredictDelay;

	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Boss;
	ALightningStrikeActor* Lightning = Boss->GetWorld()->SpawnActor<ALightningStrikeActor>(
		LightningClass, PredictedLoc, FRotator::ZeroRotator, SpawnParams);

	
	if (Lightning)
	{
		Lightning->SetInstigatorAbility(this);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
