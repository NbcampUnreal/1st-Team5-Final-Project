// PayRockGames

#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY(BlueprintAssignable)
	FOnDurationEnd OnDurationEnd;
	
	FTimerHandle SpawnTimerHandle;
	FTimerHandle StopTimerHandle;
	
};
