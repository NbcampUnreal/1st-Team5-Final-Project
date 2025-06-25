// PayRockGames


#include "OneEyedWeaponSkill.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_SpawnActor.h"
#include "PayRock/Character/BaseCharacter.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PayRock/Actor/BaseProjectile.h"

void UOneEyedWeaponSkill::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (GetWorld() && !GetWorld()->bIsTearingDown)
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}

	if (IsValid(SpawnedLightBall))
	{
		SpawnedLightBall->Destroy();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UOneEyedWeaponSkill::OnEventReceived(FGameplayEventData Payload)
{
	Super::OnEventReceived(Payload);

	if (GetWorld() && !GetWorld()->bIsTearingDown && SpawnLightBall())
	{
		GetWorld()->GetTimerManager().SetTimer(
			RepeatTimer, this, &UOneEyedWeaponSkill::AttackActorsWithinRadius, Interval, true, 1.f);
		GetWorld()->GetTimerManager().SetTimer(
			StopTimer, this, &UOneEyedWeaponSkill::K2_EndAbility, Duration);
	}
}

void UOneEyedWeaponSkill::AttackActorsWithinRadius()
{
	FVector Origin = GetAvatarActorFromActorInfo()->GetActorLocation();

	FVector Size(Radius * 2);
	FCollisionShape Box = FCollisionShape::MakeBox(Size);

	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetAvatarActorFromActorInfo());	
	QueryParams.bTraceIntoSubComponents = false;

	FCollisionObjectQueryParams CollisionParams;
	CollisionParams.AddObjectTypesToQuery(ECC_Pawn);
	
	bool bHit = GetWorld()->OverlapMultiByObjectType(
		Overlaps, Origin, FQuat::Identity, CollisionParams, Box, QueryParams);

	if (bHit)
	{
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
		if (!SourceASC) return;
		
		TSet<ABaseCharacter*> OverlapCharacters;
		for (const FOverlapResult& Result : Overlaps)
		{
			if (!IsValid(Result.GetActor())) return;
			if (ABaseCharacter* OverlapCharacter = Cast<ABaseCharacter>(Result.GetActor()))
			{
				if (!OverlapCharacters.Contains(OverlapCharacter))
				{
					CauseDamage(OverlapCharacter, FHitResult());
					SpawnLightBeam(OverlapCharacter->GetActorLocation());
					OverlapCharacters.Add(OverlapCharacter);
				}
			}
		}
	}
}

bool UOneEyedWeaponSkill::SpawnLightBall()
{
	AActor* SourceActor = GetAvatarActorFromActorInfo();
	FGameplayAbilityTargetDataHandle TargetDataHandle =
		UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(SourceActor);
	
	UAbilityTask_SpawnActor* SpawnActorTask =
		UAbilityTask_SpawnActor::SpawnActor(this, TargetDataHandle, LightBallClass);

	if (SpawnActorTask)
	{
		bool bDidBeginSpawn = SpawnActorTask->BeginSpawningActor(
			this, TargetDataHandle, LightBallClass, SpawnedLightBall);
		SpawnActorTask->FinishSpawningActor(this, TargetDataHandle, SpawnedLightBall);
		
		if (bDidBeginSpawn && IsValid(SpawnedLightBall))
		{
			SpawnedLightBall->AttachToActor(SourceActor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			SpawnedLightBall->AddActorLocalOffset(FVector(0, 0, LightBallZOffset));
		}
		return bDidBeginSpawn;
	}
	return false;
}

void UOneEyedWeaponSkill::SpawnLightBeam(const FVector& TargetLoc)
{
	AActor* SourceActor = GetAvatarActorFromActorInfo();
	FGameplayAbilityTargetDataHandle TargetDataHandle =
		UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(SourceActor);
	
	UAbilityTask_SpawnActor* SpawnActorTask =
		UAbilityTask_SpawnActor::SpawnActor(this, TargetDataHandle, LightBeamClass);

	if (SpawnActorTask)
	{
		AActor* SpawnedLightBeam = nullptr;
		bool bDidBeginSpawn = SpawnActorTask->BeginSpawningActor(
			this, TargetDataHandle, LightBeamClass, SpawnedLightBeam);
		
		SpawnActorTask->FinishSpawningActor(this, TargetDataHandle, SpawnedLightBeam);
		if (bDidBeginSpawn && IsValid(SpawnedLightBeam))
		{
			SpawnedLightBeam->AddActorLocalOffset(FVector(0.f, 0.f, LightBallZOffset));

			FVector Direction = TargetLoc - SpawnedLightBeam->GetActorLocation();
			SpawnedLightBeam->SetActorRotation(Direction.ToOrientationQuat());
		}
	}
}
