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


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timing")
	float AuraDelayTime = 0.8f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	TSubclassOf<AFireballProjectile> FireballClass;
	
	FTimerHandle AuraDelayTimerHandle;
	FTimerHandle FireballSequenceTimerHandle;
	
	int32 TotalFireballsToSpawn = 0;
	int32 CurrentFireballIndex = 0;

	TWeakObjectPtr<AMukCheonWangCharacter> Caster;
	TArray<TWeakObjectPtr<AActor>> DetectedTargets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fireball|SpawnArea")
	float MinForwardOffset = 300.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fireball|SpawnArea")
	float MaxForwardOffset = 800.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fireball|SpawnArea")
	float SideOffsetRange = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fireball|SpawnArea")
	float VerticalOffset = 180.f;
};