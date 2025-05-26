// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "StoneSpikeActor.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "GA_VFX_StoneImpale.generated.h"

UCLASS()
class PAYROCK_API UGA_VFX_StoneImpale : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_VFX_StoneImpale();
protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, Category = "Stone")
	TSubclassOf<AStoneSpikeActor> StoneSpikeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Stone")
	float SpawnDistance = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "Stone")
	float AngleOffset = 15.f; 
};