// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "BaseWeaponAbility.generated.h"

class APRCharacter;

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	NormalAttack = 0,
	DashAttack,
	JumpAttack
};

UCLASS()
class PAYROCK_API UBaseWeaponAbility : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
protected:
	UFUNCTION(BlueprintCallable)
	void PerformSweep();
	UFUNCTION(BlueprintCallable)
	void ResetHitActors();
	
	void UpdateCurrentAttackType(APRCharacter* PlayerCharacter);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ComboTimeLimit = 0.3f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UAnimMontage>> EndingMontageArray;

	UPROPERTY(BlueprintReadOnly)
	EAttackType CurrentAttackType = EAttackType::NormalAttack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> DashAttackMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> JumpAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SweepForwardOffset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SweepDistance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SweepWidth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DashDistanceMultiplier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float JumpDistanceMultiplier;
	
	UPROPERTY()
	TSet<AActor*> AlreadyHitActors;
};
