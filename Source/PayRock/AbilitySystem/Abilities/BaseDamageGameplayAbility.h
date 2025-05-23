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
	
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor /*, const FHitResult& HitResult*/);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag DamageTypeTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UAnimMontage>> MontageArray;

	UPROPERTY(BlueprintReadOnly)
	int32 MontageIndex;

	UPROPERTY(BlueprintReadOnly)
	FHitResult HitResult;

	UPROPERTY(BlueprintReadOnly)
	float AttackSpeed = 1.f;
};
