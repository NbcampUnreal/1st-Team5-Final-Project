#include "GA_SpawnHealingTrees.h"
#include "HealingTreeActor.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"

UGA_SpawnHealingTrees::UGA_SpawnHealingTrees()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Event.Montage.Boss.Wood"));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Boss.State.Attacking"));
}

void UGA_SpawnHealingTrees::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	AvatarActor = GetAvatarActorFromActorInfo();
	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(AvatarActor);
	if (!Boss || !TreeClass)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	CurrentSpecHandle = Handle;
	CurrentActorInfo = const_cast<FGameplayAbilityActorInfo*>(ActorInfo);
	CurrentActivationInfo = ActivationInfo;

	
	PlayAuraVFX(Boss);
	
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			AuraDelayTimerHandle,
			this,
			&UGA_SpawnHealingTrees::SpawnTreesAfterAura,
			AuraDelayTime,
			false
		);
	}
}

void UGA_SpawnHealingTrees::SpawnTreesAfterAura()
{
	GetWorld()->GetTimerManager().ClearTimer(AuraDelayTimerHandle);

	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(AvatarActor);
	if (!Boss || !TreeClass)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	int32 Count = FMath::RandRange(MinSpawnCount, MaxSpawnCount);

	const FVector Forward = Boss->GetActorForwardVector();
	const FVector Right = Boss->GetActorRightVector();
	const FVector Origin = Boss->GetActorLocation();

	const float HalfWidth = SpawnAreaWidth * 0.5f;

	const FVector BoxCenter = Origin + Forward * ((MinSpawnDistance + MaxSpawnDistance) * 0.5f);
	const FVector BoxExtent = FVector((MaxSpawnDistance - MinSpawnDistance) * 0.5f, HalfWidth, 100.f);

	DrawDebugBox(GetWorld(), BoxCenter, BoxExtent, Boss->GetActorRotation().Quaternion(), FColor::Green, false, 3.f);

	for (int32 i = 0; i < Count; ++i)
	{
		const float ForwardOffset = FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);
		const float SideOffset = FMath::FRandRange(-HalfWidth, HalfWidth);

		FVector Offset = (Forward * ForwardOffset) + (Right * SideOffset);
		FVector SpawnLoc = Origin + Offset;
		SpawnLoc.Z += 50.f;

		FActorSpawnParameters Params;
		Params.Owner = Boss;

		AHealingTreeActor* Tree = Boss->GetWorld()->SpawnActor<AHealingTreeActor>(
			TreeClass, SpawnLoc, FRotator::ZeroRotator, Params);

		if (Tree)
		{
			Tree->SetBoss(Boss);
		}
	}


	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
