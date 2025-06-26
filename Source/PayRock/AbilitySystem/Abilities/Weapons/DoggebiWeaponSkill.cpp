// PayRockGames


#include "DoggebiWeaponSkill.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_SpawnActor.h"
#include "PayRock/PRGameplayTags.h"
#include "PayRock/Actor/DoggebiSpinActor.h"

void UDoggebiWeaponSkill::OnEventReceived(FGameplayEventData Payload)
{
	Super::OnEventReceived(Payload);

	if (GetWorld() && !GetWorld()->bIsTearingDown && SpawnDoggebiSpinActor())
	{
		GetWorld()->GetTimerManager().SetTimer(
			StopTimer, this, &UDoggebiWeaponSkill::K2_EndAbility, Duration);

		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			ASC->RegisterGameplayTagEvent(FPRGameplayTags::Get().Ability_WeaponSkill_Doggebi).RemoveAll(this);
			ASC->RegisterGameplayTagEvent(FPRGameplayTags::Get().Ability_WeaponSkill_Doggebi).AddUObject(
				this, &UDoggebiWeaponSkill::TriggerSpinAttack);
		}
	}
}

void UDoggebiWeaponSkill::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (GetWorld() && !GetWorld()->bIsTearingDown && IsValid(SpawnedActor))
	{
		GetWorld()->GetTimerManager().ClearTimer(StopTimer);
		SpawnedActor->Destroy();
	}
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		ASC->RegisterGameplayTagEvent(FPRGameplayTags::Get().Ability_WeaponSkill_Doggebi).RemoveAll(this);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDoggebiWeaponSkill::TriggerSpinAttack(FGameplayTag Tag, int Count)
{
	if (Count > 0)
	{
		if (ADoggebiSpinActor* DoggebiSpinActor = Cast<ADoggebiSpinActor>(SpawnedActor))
		{
			DoggebiSpinActor->StartAttack();
		}

		/*if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			FGameplayTagContainer Tags;
			Tags.AddTagFast(FPRGameplayTags::Get().Ability_WeaponSkill_Doggebi);
			ASC->RemoveActiveEffectsWithGrantedTags(Tags);
		}*/
	}
}

bool UDoggebiWeaponSkill::SpawnDoggebiSpinActor()
{
	AActor* SourceActor = GetAvatarActorFromActorInfo();
	FGameplayAbilityTargetDataHandle TargetDataHandle =
		UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(SourceActor);
	
	UAbilityTask_SpawnActor* SpawnActorTask =
		UAbilityTask_SpawnActor::SpawnActor(this, TargetDataHandle, SpawnClass);

	if (SpawnActorTask)
	{
		bool bDidBeginSpawn = SpawnActorTask->BeginSpawningActor(
			this, TargetDataHandle, SpawnClass, SpawnedActor);
		
		SpawnActorTask->FinishSpawningActor(this, TargetDataHandle, SpawnedActor);
		
		if (bDidBeginSpawn && IsValid(SpawnedActor))
		{
			if (ADoggebiSpinActor* DoggebiSpinActor = Cast<ADoggebiSpinActor>(SpawnedActor))
			{
				DoggebiSpinActor->SourceActor = SourceActor;
				DoggebiSpinActor->SourceDamageAbility = this;
			}	
		}
		return bDidBeginSpawn;
	}
	return false;
}
