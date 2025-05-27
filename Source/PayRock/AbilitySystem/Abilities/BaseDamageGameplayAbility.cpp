// PayRockGames


#include "BaseDamageGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"

void UBaseDamageGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (const UPRAttributeSet* AttributeSet = Cast<UPRAttributeSet>(
		GetAbilitySystemComponentFromActorInfo()->GetAttributeSet(UPRAttributeSet::StaticClass())))
	{
		AttackSpeed = AttributeSet->GetAttackSpeed() / 100.f;
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UBaseDamageGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (MontageArray.Num() > 0)
	{
		MontageIndex = (MontageIndex + 1) % MontageArray.Num();		
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UBaseDamageGameplayAbility::CauseDamage(AActor* TargetActor /*, const FHitResult& InHitResult*/)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;
	// HitResult = InHitResult;
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
		const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
			DamageEffectSpecHandle, DamageTypeTag, ScaledDamage);
		GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
			*DamageEffectSpecHandle.Data.Get(), TargetASC);
	}
}

void UBaseDamageGameplayAbility::PlayAuraVFX(AActor* TargetActor)
{
	if (AuraEffect && TargetActor)
	{
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
			AuraEffect,
			TargetActor->GetRootComponent(),
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);

		if (NiagaraComp)
		{
			FTimerHandle DestroyHandle;
			FTimerDelegate DestroyDelegate = FTimerDelegate::CreateLambda([NiagaraComp]()
			{
				NiagaraComp->DestroyComponent();
			});

			TargetActor->GetWorldTimerManager().SetTimer(
				DestroyHandle,
				DestroyDelegate,
				AuraRate,
				false
			);
		}
	}

	if (AuraDecalClass && TargetActor && TargetActor->GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = TargetActor;

		const FVector SpawnLocation = TargetActor->GetActorLocation();
		const FRotator SpawnRotation = FRotator::ZeroRotator;

		AActor* AuraDecal = TargetActor->GetWorld()->SpawnActor<AActor>(
			AuraDecalClass,
			SpawnLocation,
			SpawnRotation,
			SpawnParams
		);

		if (AuraDecal)
		{
			AuraDecal->AttachToActor(TargetActor, FAttachmentTransformRules::KeepWorldTransform);
			// 자동 파괴는 액터 클래스 내부에서 관리
		}
	}
}
