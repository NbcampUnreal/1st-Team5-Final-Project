// PayRockGames


#include "BossWeaponFireSkill.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BossWeaponFireProjectile.h"
#include "Abilities/Tasks/AbilityTask_SpawnActor.h"

void UBossWeaponFireSkill::OnEventReceived(FGameplayEventData Payload)
{
	Super::OnEventReceived(Payload);

	TArray<AActor*> Overlaps;
	GetSphereOverlapResults(Radius, Overlaps);

	int32 Index = 0;
	for (AActor* Target : Overlaps)
	{
		if (Index >= MaxProjectiles)
		{
			K2_EndAbility();
			return;
		}
		
		if (!IsValid(Target)) continue;

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
				if (ABossWeaponFireProjectile* SpawnedProjectile = Cast<ABossWeaponFireProjectile>(SpawnedActor))
				{
					SpawnedProjectile->SourceAbility = this;
					SpawnedProjectile->SourceActor = SourceActor;
					SpawnedProjectile->TargetActor = Target;
				}
			}
			SpawnActorTask->FinishSpawningActor(this, TargetDataHandle, SpawnedActor);
			++Index;
		}
	}
}
