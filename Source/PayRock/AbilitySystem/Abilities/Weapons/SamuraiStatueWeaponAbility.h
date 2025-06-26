// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseMontageDamageSkill.h"
#include "SamuraiStatueWeaponAbility.generated.h"

UCLASS()
class PAYROCK_API USamuraiStatueWeaponAbility : public UBaseMontageDamageSkill
{
	GENERATED_BODY()

public:
	virtual void OnEventReceived(FGameplayEventData Payload) override;

protected:
	UFUNCTION()
	TSet<AActor*> GetActorsFacingAvatar();

	UPROPERTY(EditDefaultsOnly)
	float Radius = 300.f;
	UPROPERTY(EditDefaultsOnly)
	float AngleThreshold = 45.f;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> EffectToApply;
};
