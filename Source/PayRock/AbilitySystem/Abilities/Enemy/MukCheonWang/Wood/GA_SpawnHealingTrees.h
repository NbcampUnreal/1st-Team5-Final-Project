// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "HealingTreeActor.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "GA_SpawnHealingTrees.generated.h"

UCLASS()
class PAYROCK_API UGA_SpawnHealingTrees : public UBaseDamageGameplayAbility
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tree")
	TSubclassOf<AHealingTreeActor> TreeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tree")
	int32 MinSpawnCount = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tree")
	int32 MaxSpawnCount = 4;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tree")
	float MinSpawnDistance = 300.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tree")
	float MaxSpawnDistance = 2000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tree")
	float SpawnAreaWidth = 700.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timing")
	float AuraDelayTime = 0.8f;

	FTimerHandle AuraDelayTimerHandle;

	UFUNCTION()
	void SpawnTreesAfterAura();

	FGameplayAbilitySpecHandle CurrentSpecHandle;
	FGameplayAbilityActorInfo* CurrentActorInfo = nullptr;
	FGameplayAbilityActivationInfo CurrentActivationInfo;

	UPROPERTY()
	TObjectPtr<AActor> AvatarActor;
};
