// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "GA_ChargeStamp.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UGA_ChargeStamp : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_ChargeStamp();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	
	
protected:

	UPROPERTY(EditDefaultsOnly)
	float ChargeTime = 2.0f;

	UPROPERTY(EditDefaultsOnly)
	float JumpForce = 10.f;

	UPROPERTY(EditDefaultsOnly)
	float DamageRadius = 250.f;

	FVector TargetLocation;

	

	void StartChargeVisual(AActor* Avatar);
	void JumpToTarget(ACharacter* Avatar);
	void ApplyStampDamage(ACharacter* Avatar);
};
