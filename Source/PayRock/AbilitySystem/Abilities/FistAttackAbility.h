// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseDamageGameplayAbility.h"
#include "FistAttackAbility.generated.h"


UCLASS()
class PAYROCK_API UFistAttackAbility : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
protected:
	UFUNCTION(BlueprintCallable)
	void BindCallbackToFistCollision();
	
	UFUNCTION(BlueprintCallable)
	void ToggleFistCollision(bool bShouldEnable);

	UFUNCTION()
	void OnFistOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class APRCharacter> AvatarCharacter;

	bool bHit = false;
};
