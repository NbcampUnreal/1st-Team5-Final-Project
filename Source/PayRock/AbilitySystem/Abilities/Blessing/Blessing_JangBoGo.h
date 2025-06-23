// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "Blessing_JangBoGo.generated.h"

class ABaseProjectile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDurationEnd);

UCLASS()
class PAYROCK_API UBlessing_JangBoGo : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void SpawnCannonBall();

	UFUNCTION()
	void StopSpawning();
	UFUNCTION()
	void OnTagReceived(FGameplayEventData Payload);
	
	UPROPERTY(EditDefaultsOnly, Category = "Blessing")
	float SpawnInterval = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Blessing")
	FScalableFloat Duration;

	UPROPERTY(EditDefaultsOnly, Category = "Blessing")
	FScalableFloat Radius;

	UPROPERTY(EditDefaultsOnly, Category = "Blessing")
	TSubclassOf<ABaseProjectile> CannonBallClass;

	UPROPERTY(EditDefaultsOnly, Category = "Blessing")
	TObjectPtr<UAnimMontage> EndMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Blessing")
	TObjectPtr<UAnimMontage> ActivationMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Blessing")
	FGameplayTag StartSpawnTag;

	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* WaitEventTask;
	UPROPERTY()
	UAbilityTask_PlayMontageAndWait* MontageTask;

	UPROPERTY(BlueprintAssignable)
	FOnDurationEnd OnDurationEnd;
	
	FTimerHandle SpawnTimerHandle;
	FTimerHandle StopTimerHandle;
	
};
