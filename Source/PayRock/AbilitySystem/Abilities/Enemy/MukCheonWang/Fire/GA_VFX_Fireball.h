// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "FireballProjectile.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"
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
		const FGameplayEventData* TriggerEventData) override;

protected:
	void StartFireballSequence();
	
	void SpawnNextFireball();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	TSubclassOf<AFireballProjectile> FireballClass;
	

	FTimerHandle AuraDelayTimerHandle;
	FTimerHandle FireballSequenceTimerHandle;
	
	int32 TotalFireballsToSpawn = 0;
	int32 CurrentFireballIndex = 0;

	TWeakObjectPtr<AMukCheonWangCharacter> Caster;
	TArray<TWeakObjectPtr<AActor>> DetectedTargets;
};