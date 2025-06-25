// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "PayRock/AbilitySystem/Abilities/BaseMontageDamageSkill.h"
#include "OneEyedWeaponSkill.generated.h"

UCLASS()
class PAYROCK_API UOneEyedWeaponSkill : public UBaseMontageDamageSkill
{
	GENERATED_BODY()

public:
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void OnEventReceived(FGameplayEventData Payload) override;
	
protected:
	UFUNCTION()
	void AttackActorsWithinRadius();
	UFUNCTION()
	bool SpawnLightBall();
	UFUNCTION()
	void SpawnLightBeam(const FVector& TargetLoc);

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	float Radius;
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	float Duration;
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	float Interval;
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	float LightBallZOffset;
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<AActor> LightBallClass;
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<AActor> LightBeamClass;
	UPROPERTY()
	AActor* SpawnedLightBall = nullptr;
	
	FTimerHandle StopTimer;
	FTimerHandle RepeatTimer;
};
