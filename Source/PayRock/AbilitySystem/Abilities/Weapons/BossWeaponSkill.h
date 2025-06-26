// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseMontageDamageSkill.h"
#include "BossWeaponSkill.generated.h"

UCLASS()
class PAYROCK_API UBossWeaponSkill : public UBaseMontageDamageSkill
{
	GENERATED_BODY()

public:
	virtual void OnEventReceived(FGameplayEventData Payload) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> ActivatableAbilities;
};
