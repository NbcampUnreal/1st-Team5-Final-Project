// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseMontageDamageSkill.h"
#include "GeneralSkyWeaponSkill.generated.h"

UCLASS()
class PAYROCK_API UGeneralSkyWeaponSkill : public UBaseMontageDamageSkill
{
	GENERATED_BODY()
	
public:
	virtual void OnEventReceived(FGameplayEventData Payload) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void OnLanded(const FHitResult& Hit);

protected:
	void ActivateLandEffect();
	
	UPROPERTY(EditDefaultsOnly)
	float LaunchSpeed = 500.f;
	UPROPERTY(EditDefaultsOnly)
	float Radius = 300.f;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> KnockbackEffectClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> LandEffectClass;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> LandMontage;

	UPROPERTY()
	TSet<AActor*> UniqueActors;
};
