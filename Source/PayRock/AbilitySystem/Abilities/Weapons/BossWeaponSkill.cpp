// PayRockGames


#include "BossWeaponSkill.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_SpawnActor.h"
#include "BossWeapon/BossWeaponFireProjectile.h"
#include "PayRock/AbilitySystem/PRAbilitySystemComponent.h"
#include "PayRock/Character/BaseCharacter.h"

void UBossWeaponSkill::OnEventReceived(FGameplayEventData Payload)
{
	Super::OnEventReceived(Payload);

	if (GetAvatarActorFromActorInfo()->HasAuthority())
	{
		int32 RandomIndex = FMath::RandRange(0, static_cast<uint8>(EBossWeaponSkillType::MAX) - 1);
		switch (static_cast<EBossWeaponSkillType>(RandomIndex))
		{
		case EBossWeaponSkillType::Fire:
			ExecuteFireSkill();
			break;
		case EBossWeaponSkillType::Lightning:
			ExecuteSkill(LightningTargetEffectClass, LightningSelfEffectClass);
			break;
		case EBossWeaponSkillType::Spike:
			ExecuteSkill(SpikeTargetEffectClass, SpikeSelfEffectClass);
			break;
		case EBossWeaponSkillType::Roar:
			ExecuteRoarSkill();
			break;
		default: ;
		}
	}
	
	K2_EndAbility();
}

void UBossWeaponSkill::ExecuteFireSkill()
{
	TArray<AActor*> Overlaps;
	GetSphereOverlapResults(Radius, Overlaps);
	
	for (int i = 0; i < Overlaps.Num(); i++)
	{
		AActor* Target = Overlaps[i];
		if (!IsValid(Target)) continue;

		AActor* SourceActor = GetAvatarActorFromActorInfo();
		FGameplayAbilityTargetDataHandle TargetDataHandle =
			UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(SourceActor);
		UAbilityTask_SpawnActor* SpawnActorTask =
			UAbilityTask_SpawnActor::SpawnActor(this, TargetDataHandle, FireProjectileClass);

		if (SpawnActorTask)
		{
			AActor* SpawnedActor = nullptr;
			bool bDidBeginSpawn = SpawnActorTask->BeginSpawningActor(
				this, TargetDataHandle, FireProjectileClass, SpawnedActor);
			if (bDidBeginSpawn && IsValid(SpawnedActor))
			{
				if (ABossWeaponFireProjectile* SpawnedProjectile = Cast<ABossWeaponFireProjectile>(SpawnedActor))
				{
					SpawnedProjectile->SourceAbility = this;
					SpawnedProjectile->SourceActor = SourceActor;
					SpawnedProjectile->TargetActor = Target;
				}

				FRotator Rotation = GetAvatarActorFromActorInfo()->GetActorForwardVector().ToOrientationRotator();
				if (Overlaps.Num() > 0)
				{
					Rotation.Yaw += 360.f / Overlaps.Num() * i;
				}
				SpawnedActor->SetActorRotation(Rotation);
			}
			SpawnActorTask->FinishSpawningActor(this, TargetDataHandle, SpawnedActor);
		}
	}
}

void UBossWeaponSkill::ExecuteRoarSkill()
{
	TArray<AActor*> Overlaps;
	GetSphereOverlapResults(Radius, Overlaps);
	
	for (AActor* Target : Overlaps)
	{
		if (!IsValid(Target)) continue;
		
		if (IsValid(Target))
		{
			CauseDamage(Target, FHitResult());
		}

		if (ABaseCharacter* Character = Cast<ABaseCharacter>(Target))
		{
			FVector Direction = (Target->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).GetSafeNormal();
			Direction.Z = 1.f;
			Character->LaunchCharacter(Direction * LaunchSpeed, true, true);
		}
	}

	AActor* SourceActor = GetAvatarActorFromActorInfo();
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (IsValid(RoarSelfEffectClass) && IsValid(SourceASC))
	{
		FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
		Context.AddOrigin(SourceActor->GetActorLocation());
		FGameplayEffectSpecHandle Spec = SourceASC->MakeOutgoingSpec(RoarSelfEffectClass, 1.f, Context);
		SourceASC->ApplyGameplayEffectSpecToSelf(*Spec.Data);
	}
}

void UBossWeaponSkill::ExecuteSkill(TSubclassOf<UGameplayEffect> EffectToTarget, TSubclassOf<UGameplayEffect> EffectToSelf)
{
	TArray<AActor*> Overlaps;
	GetSphereOverlapResults(Radius, Overlaps);

	AActor* SourceActor = GetAvatarActorFromActorInfo();

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	for (AActor* Target : Overlaps)
	{
		if (IsValid(Target))
		{
			CauseDamage(Target, FHitResult());
		}

		if (!IsValid(EffectToTarget)) continue;
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
		if (!IsValid(TargetASC)) return;
		if (UPRAbilitySystemComponent* TargetPRASC = Cast<UPRAbilitySystemComponent>(TargetASC))
		{
			TargetPRASC->StoredHitDirection =
				(Target->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).GetSafeNormal();
		}
		FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
		Context.AddOrigin(SourceActor->GetActorLocation());
		FGameplayEffectSpecHandle Spec = SourceASC->MakeOutgoingSpec(EffectToTarget, 1.f, Context);
		SourceASC->ApplyGameplayEffectSpecToTarget(*Spec.Data, TargetASC);
	}

	if (IsValid(EffectToSelf))
	{
		FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
		Context.AddOrigin(SourceActor->GetActorLocation());
		FGameplayEffectSpecHandle Spec = SourceASC->MakeOutgoingSpec(EffectToSelf, 1.f, Context);
		SourceASC->ApplyGameplayEffectSpecToSelf(*Spec.Data);
	}
}
