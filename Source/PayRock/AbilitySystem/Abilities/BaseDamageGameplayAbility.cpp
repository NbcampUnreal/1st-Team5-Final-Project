// PayRockGames


#include "BaseDamageGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PayRock/PRGameplayTags.h"
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

void UBaseDamageGameplayAbility::CauseDamage(AActor* TargetActor, bool bIsBackAttack)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;

	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
		
		FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(
			DamageEffectClass, GetAbilityLevel());
		float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());

		// Back Attack Buff Bonus
		float BackAttackMultiplier = 0.f;
		if (bIsBackAttack && ASC->HasMatchingGameplayTag(FPRGameplayTags::Get().Status_Buff_BackAttack))
		{
			BackAttackMultiplier = GetBackAttackMultiplier();
		}
		ScaledDamage *= (1.f + BackAttackMultiplier);
		
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
			DamageEffectSpecHandle, DamageTypeTag, ScaledDamage);
		ASC->ApplyGameplayEffectSpecToTarget(
			*DamageEffectSpecHandle.Data.Get(), TargetASC);
	}
}

float UBaseDamageGameplayAbility::GetBackAttackMultiplier()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC) return 0.f;

	int32 BackAttackLevel = 1;
	FGameplayTagContainer Tags;
	Tags.AddTag(FPRGameplayTags::Get().Status_Buff_BackAttack);
	for (const auto& ActiveEffect : ASC->GetActiveEffectsWithAllTags(Tags))
	{
		if (!ActiveEffect.WasSuccessfullyApplied()) continue;
	
		// will return 1 if for some reason data isn't valid
		FGameplayEffectContextHandle ContextHandle = ASC->GetEffectContextFromActiveGEHandle(ActiveEffect);
		BackAttackLevel = ContextHandle.GetAbilityLevel();
		break; // Presuming there's only one GA that granted this buff
	}
	// TODO : make this not hard coding
	switch (BackAttackLevel)
	{
	case 1:		return 0.5f;
	case 2:		return 0.75f;
	case 3:		return 1.f;
	case 4:		return 1.5f;
	default:	return 1.f;
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
