// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "MarketClownWeaponSkill.generated.h"


class ABaseProjectile;

UCLASS()
class PAYROCK_API UMarketClownWeaponSkill : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void SpawnProjectile(const FVector& Direction);

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<ABaseProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	int32 NumProjectiles;
	
	/*UFUNCTION()
	void OnTagReceived(FGameplayEventData Payload);
	
	UPROPERTY(EditDefaultsOnly, Category = "Blessing")
	float SpawnInterval = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Blessing")
	FScalableFloat Duration;

	UPROPERTY(EditDefaultsOnly, Category = "Blessing")
	FScalableFloat Radius;

	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* WaitEventTask;
	
	UPROPERTY()
	UAbilityTask_PlayMontageAndWait* MontageTask;

	UPROPERTY(BlueprintAssignable)
	FOnDurationEnd OnDurationEnd;
	
	FTimerHandle SpawnTimerHandle;
	FTimerHandle StopTimerHandle;*/
	
};
