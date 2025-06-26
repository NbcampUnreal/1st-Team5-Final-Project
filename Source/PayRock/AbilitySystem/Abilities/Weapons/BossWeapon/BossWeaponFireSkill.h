// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseMontageDamageSkill.h"
#include "BossWeaponFireSkill.generated.h"

class ABossWeaponFireProjectile;

UCLASS()
class PAYROCK_API UBossWeaponFireSkill : public UBaseMontageDamageSkill
{
	GENERATED_BODY()

public:
	virtual void OnEventReceived(FGameplayEventData Payload) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	float Radius = 300.f;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABossWeaponFireProjectile> ProjectileClass;
	UPROPERTY(EditDefaultsOnly)
	int32 MaxProjectiles = 8;
};
