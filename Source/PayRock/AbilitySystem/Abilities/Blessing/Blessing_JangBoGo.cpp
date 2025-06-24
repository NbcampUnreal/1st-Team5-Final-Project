// PayRockGames


#include "Blessing_JangBoGo.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_SpawnActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PayRock/Actor/BaseProjectile.h"


void UBlessing_JangBoGo::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !GetWorld() || GetWorld()->bIsTearingDown)
	{			
		constexpr bool bReplicateEndAbility = true;
		constexpr bool bWasCancelled = true;
		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		return;
	}

	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, ActivationMontage);
	MontageTask->ReadyForActivation();

	WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, StartSpawnTag, nullptr, false, true);
	WaitEventTask->EventReceived.AddDynamic(this, &UBlessing_JangBoGo::OnTagReceived);
	WaitEventTask->ReadyForActivation();

	GetWorld()->GetTimerManager().SetTimer(
		StopTimerHandle,
		this,
		&UBlessing_JangBoGo::StopSpawning,
		Duration.GetValueAtLevel(GetAbilityLevel())
	);	
}
void UBlessing_JangBoGo::OnTagReceived(FGameplayEventData Payload)
{
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&UBlessing_JangBoGo::SpawnCannonBall,
		SpawnInterval,
		true,
		0.f);
}
void UBlessing_JangBoGo::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (!GetWorld() || GetWorld()->bIsTearingDown || !IsValid(this)) return;
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void UBlessing_JangBoGo::SpawnCannonBall()
{
	AActor* SourceActor = GetAvatarActorFromActorInfo();
	FGameplayAbilityTargetDataHandle TargetDataHandle =
		UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(SourceActor);
	
	UAbilityTask_SpawnActor* SpawnActorTask =
		UAbilityTask_SpawnActor::SpawnActor(this, TargetDataHandle, CannonBallClass);

	if (SpawnActorTask)
	{
		AActor* SpawnedActor = nullptr;
		bool bDidBeginSpawn = SpawnActorTask->BeginSpawningActor(
			this, TargetDataHandle, CannonBallClass, SpawnedActor);
		if (bDidBeginSpawn && IsValid(SpawnedActor))
		{
			float RadiusFloat = Radius.GetValueAtLevel(GetAbilityLevel());
			FVector2D RandomOffset2D = FMath::RandPointInCircle(RadiusFloat);
			SpawnedActor->AddActorLocalOffset(FVector(RandomOffset2D.X, RandomOffset2D.Y, 100.f));

			if (ABaseProjectile* SpawnedProjectile = Cast<ABaseProjectile>(SpawnedActor))
			{
				SpawnedProjectile->SourceAbility = this;
				SpawnedProjectile->SourceActor = GetAvatarActorFromActorInfo();
			}
		}
		SpawnActorTask->FinishSpawningActor(this, TargetDataHandle, SpawnedActor);
	}
}

void UBlessing_JangBoGo::StopSpawning()
{
	if (!GetWorld() || GetWorld()->bIsTearingDown)
	{
		K2_EndAbility();
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);

	OnDurationEnd.Broadcast();
}
