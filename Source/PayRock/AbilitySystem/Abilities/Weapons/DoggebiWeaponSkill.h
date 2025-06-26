// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "PayRock/AbilitySystem/Abilities/BaseMontageDamageSkill.h"
#include "DoggebiWeaponSkill.generated.h"


UCLASS()
class PAYROCK_API UDoggebiWeaponSkill : public UBaseMontageDamageSkill
{
	GENERATED_BODY()

public:
	
	virtual void OnEventReceived(FGameplayEventData Payload) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void TriggerSpinAttack(FGameplayTag Tag, int Count);
	bool SpawnDoggebiSpinActor();

	UPROPERTY(EditDefaultsOnly)
	float Duration;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> SpawnClass;
	UPROPERTY(EditDefaultsOnly)
	FVector SpawnOffset;
	UPROPERTY()
	AActor* SpawnedActor;
	
	
	FTimerHandle StopTimer;
};
