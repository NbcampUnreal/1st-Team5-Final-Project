// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "FireballProjectile.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "GA_VFX_Fireball.generated.h"


UCLASS()
class PAYROCK_API UGA_VFX_Fireball : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_VFX_Fireball();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fireball")
	TSubclassOf<AFireballProjectile> FireballClass;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timing")
	float AuraDelayTime = 0.8f;


	FTimerHandle AuraDelayTimerHandle;

	
	UFUNCTION()
	void SpawnFireballAfterAura();
	
	FGameplayAbilitySpecHandle CurrentSpecHandle;
	const FGameplayAbilityActorInfo* CurrentActorInfo = nullptr;
	FGameplayAbilityActivationInfo CurrentActivationInfo;

	UPROPERTY()
	TObjectPtr<AActor> AvatarActor;
};