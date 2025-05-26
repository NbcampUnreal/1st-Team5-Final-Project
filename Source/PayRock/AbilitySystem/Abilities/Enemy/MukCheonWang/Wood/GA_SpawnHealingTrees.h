// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "HealingTreeActor.h"
#include "PayRock/AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "GA_SpawnHealingTrees.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UGA_SpawnHealingTrees : public UBaseGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_SpawnHealingTrees();

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, Category = "Tree")
	TSubclassOf<AHealingTreeActor> TreeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Tree")
	int32 MinSpawnCount = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Tree")
	int32 MaxSpawnCount = 4;

	UPROPERTY(EditDefaultsOnly, Category = "Tree")
	float SpawnRadius = 1000.f;

	

	
};