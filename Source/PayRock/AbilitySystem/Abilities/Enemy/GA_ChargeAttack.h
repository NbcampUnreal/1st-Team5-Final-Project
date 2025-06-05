// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "PayRock/Enemy/SpecialEnemy/JangSanTiger/JasnSanTiger.h"
#include "GA_ChargeAttack.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UGA_ChargeAttack : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_ChargeAttack();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	
	UFUNCTION(BlueprintCallable)
	void OnChargeStart();

	UFUNCTION(BlueprintCallable)
	void DoChargeSweep();
	
	UFUNCTION(BlueprintCallable)
	void EndCharge();

	FTimerHandle SweepTimerHandle;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Charge")
	float TelegraphDuration = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Charge")
	float ChargeSpeed = 3000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Charge")
	float ChargeDuration = 1.0f;
	

private:
	UPROPERTY()
	AJasnSanTiger* Tiger;

	FTimerHandle TelegraphTimerHandle;
	FTimerHandle EndChargeTimerHandle;

	UPROPERTY()
	TSet<AActor*> HitActors;
	UPROPERTY(EditDefaultsOnly, Category = "Collsion Box")
	FVector BoxExtent = {300.0f,100.0f,200.0f};
	
};
