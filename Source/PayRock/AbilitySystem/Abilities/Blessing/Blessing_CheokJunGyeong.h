// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseAreaEffectAbility.h"
#include "Blessing_CheokJunGyeong.generated.h"


UCLASS(Blueprintable)
class PAYROCK_API UBlessing_CheokJunGyeong : public UBaseAreaEffectAbility
{
	GENERATED_BODY()

protected:
	virtual void RemoveEffectArea() override;
	
	UFUNCTION(BlueprintCallable)
	void OnStartEventReceived(FGameplayEventData Payload);

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* SpinAttackMontage;

	bool bSpawned;
};
