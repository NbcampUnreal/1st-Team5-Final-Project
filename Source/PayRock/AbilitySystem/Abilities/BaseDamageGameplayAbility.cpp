// PayRockGames


#include "BaseDamageGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PayRock/Character/PRCharacter.h"
#include "PayRock/PRGameplayTags.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"
#include "PayRock/Enemy/EnemyCharacter.h"
#include "Perception/AISense_Damage.h"

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

void UBaseDamageGameplayAbility::CauseDamage(AActor* TargetActor, const FHitResult& HitResult, bool bIsBackAttack)
{
	if (!IsValid(GetAvatarActorFromActorInfo()) || !IsValid(DamageEffectClass)) return;
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (!IsValid(SourceASC) || !IsValid(TargetASC)) return;

	float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());

	if (TargetASC->HasMatchingGameplayTag(FPRGameplayTags::Get().Ability_Guard))
	{
		UE_LOG(LogTemp, Warning, TEXT("GUARD SUCCESS!"));
		return;
	}

	bool bIsMonsterToMonster = GetAvatarActorFromActorInfo()->IsA(AEnemyCharacter::StaticClass()) &&
		TargetActor->IsA(AEnemyCharacter::StaticClass());
	if (bIsMonsterToMonster)
	{
		ScaledDamage *= 0.5f;
	}

	if (bUseComboDamageMultiplier)
	{
		float ComboMultiplier = DamageMultipliersPerMontage.IsValidIndex(MontageIndex)
			? DamageMultipliersPerMontage[MontageIndex]
			: DefaultComboMultiplier;
		ScaledDamage *= ComboMultiplier;
	}

	float BackAttackMultiplier = 0.f;
	if (bIsBackAttack && SourceASC->HasMatchingGameplayTag(FPRGameplayTags::Get().Status_Buff_BackAttack))
	{
		BackAttackMultiplier = GetBackAttackMultiplier();
	}
	ScaledDamage *= (1.f + BackAttackMultiplier);

	FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
	if (HitResult.bBlockingHit)
	{
		ContextHandle.AddHitResult(HitResult);
	}
	FGameplayEffectSpecHandle DamageEffectSpecHandle = SourceASC->MakeOutgoingSpec(
		DamageEffectClass, GetAbilityLevel(), ContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		DamageEffectSpecHandle, DamageTypeTag, ScaledDamage);
	SourceASC->ApplyGameplayEffectSpecToTarget(*DamageEffectSpecHandle.Data.Get(), TargetASC);

	if (AActor* SourceActor = SourceASC->GetAvatarActor())
	{
		if (APRCharacter* SourceCharacter = Cast<APRCharacter>(SourceActor))
		{
			SourceCharacter->Client_PlayHitMarker(); // 화면 중앙 히트마커
		}
	}

	UAISense_Damage::ReportDamageEvent(
		GetWorld(),
		TargetActor,
		GetAvatarActorFromActorInfo(),
		ScaledDamage,
		TargetActor->GetActorLocation(),
		GetAvatarActorFromActorInfo()->GetActorLocation()
	);
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

