// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "CycloneActor.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "GA_Cyclone.generated.h"


UCLASS()
class PAYROCK_API UGA_Cyclone : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Cyclone();
	
	
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void SpawnCycloneVDB();


	UFUNCTION()
	void OnCycloneEnded();

protected:
	
	FTimerHandle SpawnTimerHandle;


	FGameplayAbilitySpecHandle CurrentSpecHandle;
	FGameplayAbilityActorInfo* CurrentActorInfo = nullptr;
	FGameplayAbilityActivationInfo CurrentActivationInfo;

	
	UPROPERTY()
	TObjectPtr<AActor> AvatarActor;

	
	UPROPERTY(EditDefaultsOnly, Category = "Cyclone")
	TSubclassOf<ACycloneActor> CycloneClass;

	
	UPROPERTY()
	TObjectPtr<ACycloneActor> SpawnedCyclone;


	UPROPERTY(EditDefaultsOnly, Category = "Cyclone")
	float ForwardOffset = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Cyclone")
	float SpawnDelay = 0.5f;
};