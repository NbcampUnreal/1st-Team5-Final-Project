// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_BlessingCooldown.generated.h"

UCLASS()
class PAYROCK_API UMMC_BlessingCooldown : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_BlessingCooldown();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition CooldownReductionDef;

	float BaseMagnitude = 60.f;
};
