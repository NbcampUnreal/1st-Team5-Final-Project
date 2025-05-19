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

	UFUNCTION()
	void OnChargeHit(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 BodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	
	UFUNCTION(BlueprintCallable)
	void EndCharge();

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

	
};
