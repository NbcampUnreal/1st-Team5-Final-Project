#include "GA_SpawnHealingTrees.h"

#include "HealingTreeActor.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"
#include "Kismet/KismetMathLibrary.h"

UGA_SpawnHealingTrees::UGA_SpawnHealingTrees()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Event.Montage.Boss.Tree"));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Attacking"));
}

void UGA_SpawnHealingTrees::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* Avatar = GetAvatarActorFromActorInfo();
	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(Avatar);
	if (!Boss || !TreeClass)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	int32 Count = FMath::RandRange(MinSpawnCount, MaxSpawnCount);

	for (int32 i = 0; i < Count; ++i)
	{
		FVector RandOffset = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(300.f, SpawnRadius);
		FVector SpawnLoc = Boss->GetActorLocation() + RandOffset;
		SpawnLoc.Z += 50.f; // optional offset

		FActorSpawnParameters Params;
		Params.Owner = Boss;

		AHealingTreeActor* Tree = Boss->GetWorld()->SpawnActor<AHealingTreeActor>(TreeClass, SpawnLoc, FRotator::ZeroRotator, Params);
		if (Tree)
		{
			Tree->SetBoss(Boss);
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
