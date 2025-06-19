// PayRockGames

#include "BaseAreaEffectAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_SpawnActor.h"
#include "Engine/OverlapResult.h"
#include "PayRock/Actor/ApplyEffectZone.h"
#include "PayRock/Enemy/EnemyCharacter.h"


void UBaseAreaEffectAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{			
		constexpr bool bReplicateEndAbility = true;
		constexpr bool bWasCancelled = true;
		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		return;
	}
	
	if (!bActivateImmediately) return;

	if (IsValid(AdditionalEffectToApplyToSelf))
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(
			AdditionalEffectToApplyToSelf, GetAbilityLevel());
		ActiveSelfEffectHandle = ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(),
			GetCurrentActorInfo(), GetCurrentActivationInfo(), SpecHandle);
	}
	
	float DurationFloat = Duration.GetValueAtLevel(GetAbilityLevel());
	if (DurationFloat > 0.f)
	{
		SpawnEffectArea();
	}
	else
	{
		ApplyEffectToActorsWithinRadius();
	}
}

void UBaseAreaEffectAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(EffectToApplyToSelfOnEnd) && !ActiveEndEffectHandle.IsValid() && bEndAbilityOnDurationEnd)
	{
		if (UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo())
		{
			FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
			ContextHandle.AddOrigin(GetAvatarActorFromActorInfo()->GetActorLocation());
			FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
				EffectToApplyToSelfOnEnd, GetAbilityLevel(), ContextHandle);
			ActiveEndEffectHandle = ApplyGameplayEffectSpecToOwner(
				GetCurrentAbilitySpecHandle(),
				GetCurrentActorInfo(),
				GetCurrentActivationInfo(),
				SpecHandle);
		}
	}

	UniqueActors.Empty();
	
	if (IsValid(SpawnedActor))
	{
		SpawnedActor->Destroy();
		SpawnedActor = nullptr;
	}

	if (TimerHandle.IsValid() && GetWorld() && !GetWorld()->bIsTearingDown)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UBaseAreaEffectAbility::SpawnEffectArea()
{
	if (!IsValid(EffectToApplyToOverlapActors)) return;
	
	AActor* SourceActor = GetAvatarActorFromActorInfo();
	FGameplayAbilityTargetDataHandle TargetDataHandle =
		UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(SourceActor);
	
	UAbilityTask_SpawnActor* SpawnActorTask =
		UAbilityTask_SpawnActor::SpawnActor(this, TargetDataHandle, AreaActorClass);

	if (SpawnActorTask)
	{
		bool bDidBeginSpawn = SpawnActorTask->BeginSpawningActor(
			this, TargetDataHandle, AreaActorClass, SpawnedActor);
		if (bDidBeginSpawn && IsValid(SpawnedActor))
		{
			if (AApplyEffectZone* SpawnedArea = Cast<AApplyEffectZone>(SpawnedActor))
			{
				FGameplayEffectSpecHandle SpecHandle =
					MakeOutgoingGameplayEffectSpec(EffectToApplyToOverlapActors, GetAbilityLevel());
				if (bIsSetByCaller)
				{
					UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
					SpecHandle, SetByCallerTag, SetByCallerMagnitude.GetValueAtLevel(GetAbilityLevel()));	
				}
				SpawnedArea->EffectSpecHandle = SpecHandle;
				if (APawn* AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo()))
				{
					SpawnedArea->SetInstigator(AvatarPawn);
				}
			}
		}
		SpawnActorTask->FinishSpawningActor(this, TargetDataHandle, SpawnedActor);

		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle, this, &UBaseAreaEffectAbility::RemoveEffectArea, Duration.GetValueAtLevel(GetAbilityLevel()));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create UAbilityTask_SpawnActor!"));
	}
}

void UBaseAreaEffectAbility::ApplyEffectToActorsWithinRadius()
{
	if (!IsValid(EffectToApplyToOverlapActors)) return;
	
	FVector Origin = GetAvatarActorFromActorInfo()->GetActorLocation();
	float RadiusFloat = Radius.GetValueAtLevel(GetAbilityLevel());

	FCollisionShape Sphere = FCollisionShape::MakeSphere(RadiusFloat);

	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams QueryParams;
	if (bIgnoreSelf)
	{
		QueryParams.AddIgnoredActor(GetAvatarActorFromActorInfo());	
	}
	QueryParams.bTraceIntoSubComponents = false;

	FCollisionObjectQueryParams CollisionParams;
	CollisionParams.AddObjectTypesToQuery(ECC_Pawn);
	
	bool bHit = GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		Origin,
		FQuat::Identity,
		CollisionParams,
		Sphere,
		QueryParams
	);
	
	if (bHit)
	{
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
		if (!SourceASC) return;
		
		for (const FOverlapResult& Result : Overlaps)
		{
			if (AActor* Actor = Result.GetActor())
			{
				if (UniqueActors.Contains(Actor)) continue;
				if (!bIncludeAICharacters && Actor->IsA<AEnemyCharacter>()) continue;

				UniqueActors.Add(Actor);
				
				if (UAbilitySystemComponent* TargetASC =
					UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor))
				{
					FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
					ContextHandle.AddOrigin(Actor->GetActorLocation());
					FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
						EffectToApplyToOverlapActors, GetAbilityLevel(), ContextHandle);
					if (bIsSetByCaller)
					{
						UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
						SpecHandle, SetByCallerTag, SetByCallerMagnitude.GetValueAtLevel(GetAbilityLevel()));	
					}
					SourceASC->ApplyGameplayEffectSpecToTarget(
						*SpecHandle.Data.Get(),
						TargetASC
					);
				/*ApplyGameplayEffectSpecToTarget(
					GetCurrentAbilitySpecHandle(),
					GetCurrentActorInfo(),
					GetCurrentActivationInfo(),
					SpecHandle,
					UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(Actor));*/	
				}
			}
		}
	}

	if (bEndAbilityOnDurationEnd)
	{
		K2_EndAbility();
	}
	else if (IsValid(EffectToApplyToSelfOnEnd))
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(
			EffectToApplyToSelfOnEnd, GetAbilityLevel());
		ActiveEndEffectHandle = ApplyGameplayEffectSpecToOwner(
			GetCurrentAbilitySpecHandle(),
			GetCurrentActorInfo(),
			GetCurrentActivationInfo(),
			SpecHandle);
	}
}

void UBaseAreaEffectAbility::RemoveEffectArea()
{
	if (!IsValid(SpawnedActor)) return;
	SpawnedActor->Destroy();
	SpawnedActor = nullptr;

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	if (bEndAbilityOnDurationEnd)
	{
		K2_EndAbility();
	}
	else if (IsValid(EffectToApplyToSelfOnEnd))
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(
			EffectToApplyToSelfOnEnd, GetAbilityLevel());
		ActiveEndEffectHandle = ApplyGameplayEffectSpecToOwner(
			GetCurrentAbilitySpecHandle(),
			GetCurrentActorInfo(),
			GetCurrentActivationInfo(),
			SpecHandle);
	}
}

