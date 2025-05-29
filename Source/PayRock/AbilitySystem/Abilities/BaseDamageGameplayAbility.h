// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseGameplayAbility.h"
#include "BaseDamageGameplayAbility.generated.h"

UCLASS()
class PAYROCK_API UBaseDamageGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor, bool bIsBackAttack = false /*, const FHitResult& HitResult*/);

	UFUNCTION(BlueprintCallable, Category="VFX")
	void PlayAuraVFX(AActor* TargetActor);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag DamageTypeTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UAnimMontage>> MontageArray;
	
	UPROPERTY(BlueprintReadWrite)
	int32 MontageIndex;

	UPROPERTY(BlueprintReadOnly)
	FHitResult HitResult;

	UPROPERTY(BlueprintReadOnly)
	float AttackSpeed = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	TObjectPtr<UNiagaraSystem> AuraEffect;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	TSubclassOf<AActor> FontlClass;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	float AuraRate = 2.5f;

private:
	float GetBackAttackMultiplier();
};
