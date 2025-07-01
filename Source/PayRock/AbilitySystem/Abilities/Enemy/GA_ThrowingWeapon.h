// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "PayRock/Actor/Enemy/ThrowingWeapon/ThrowingWeaponBase.h"
#include "GA_ThrowingWeapon.generated.h"

/**
 * 투척 도끼를 직접 소환하는 GameplayAbility
 */
UCLASS()
class PAYROCK_API UGA_ThrowingWeapon : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_ThrowingWeapon();
	
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Throwing")
	TSubclassOf<AThrowingWeaponBase> ThrowingWeaponClass;

	
	UPROPERTY(EditDefaultsOnly, Category = "Throwing")
	float TravelSpeed = 1500.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Socket")
	FName SocketName = "LeftHandSocket";
	
	UPROPERTY(EditDefaultsOnly, Category = "Throwing")
	float VerticalLiftRatio = 0.f;
};
