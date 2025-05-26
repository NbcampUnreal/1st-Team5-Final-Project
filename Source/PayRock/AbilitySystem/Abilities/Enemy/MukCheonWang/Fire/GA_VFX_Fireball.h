// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "FireballProjectile.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "GA_VFX_Fireball.generated.h"


UCLASS()
class PAYROCK_API UGA_VFX_Fireball : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_VFX_Fireball();

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<AFireballProjectile> FireballClass;
};
