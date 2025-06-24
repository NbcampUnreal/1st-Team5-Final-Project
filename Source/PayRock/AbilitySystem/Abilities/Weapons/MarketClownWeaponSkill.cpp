// PayRockGames

#include "MarketClownWeaponSkill.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_SpawnActor.h"
#include "PayRock/Actor/BaseProjectile.h"

void UMarketClownWeaponSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || NumProjectiles <= 0) 
	{
		return;
	}
	
	const float ForwardYaw = GetAvatarActorFromActorInfo()->GetActorRotation().Yaw;
	const float AngleStep = 360.f / NumProjectiles;
	
	for (int i = 0; i < NumProjectiles; i++)
	{
		const float YawOffset = AngleStep * static_cast<float>(i);
		const FVector Direction = FRotator(0.f, ForwardYaw + YawOffset, 0.f).Vector();
		SpawnProjectile(Direction);
	}

	K2_EndAbility();
}

void UMarketClownWeaponSkill::SpawnProjectile(const FVector& Direction)
{
	AActor* SourceActor = GetAvatarActorFromActorInfo();
	FGameplayAbilityTargetDataHandle TargetDataHandle =
		UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(SourceActor);
	
	UAbilityTask_SpawnActor* SpawnActorTask =
		UAbilityTask_SpawnActor::SpawnActor(this, TargetDataHandle, ProjectileClass);

	if (SpawnActorTask)
	{
		AActor* SpawnedActor = nullptr;
		bool bDidBeginSpawn = SpawnActorTask->BeginSpawningActor(
			this, TargetDataHandle, ProjectileClass, SpawnedActor);
		if (bDidBeginSpawn && IsValid(SpawnedActor))
		{
			SpawnedActor->SetActorRotation(Direction.ToOrientationQuat());

			if (ABaseProjectile* SpawnedProjectile = Cast<ABaseProjectile>(SpawnedActor))
			{
				SpawnedProjectile->SourceAbility = this;
				SpawnedProjectile->SourceActor = GetAvatarActorFromActorInfo();
			}
		}
		SpawnActorTask->FinishSpawningActor(this, TargetDataHandle, SpawnedActor);
	}
}

