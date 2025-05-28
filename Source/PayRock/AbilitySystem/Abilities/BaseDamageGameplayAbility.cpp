// PayRockGames


#include "BaseDamageGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
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

void UBaseDamageGameplayAbility::CauseDamage(AActor* TargetActor, bool bIsBackAttack /*, const FHitResult& InHitResult*/)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;
	// HitResult = InHitResult;
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
		
		FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
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
	
	FGameplayTagContainer Tags;
	Tags.AddTag(FPRGameplayTags::Get().Status_Buff_BackAttack);
	TArray<FActiveGameplayEffectHandle> Handles = ASC->GetActiveEffectsWithAllTags(Tags);
	
	if (Handles.Num() > 0)
	{
		const FActiveGameplayEffect* Effect = ASC->GetActiveGameplayEffect(Handles[0]);
		if (Effect)
		{
			switch (static_cast<int32>(Effect->Spec.GetLevel()))
			{
			case 1:
				return 0.5f;
			case 2:
				return 0.75f;
			case 3:
				return 1.f;
			case 4:
				return 1.5f;
			}
		}
	}

	return 0.f;
}
