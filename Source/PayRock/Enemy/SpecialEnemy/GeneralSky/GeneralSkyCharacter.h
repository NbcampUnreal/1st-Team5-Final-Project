// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Enemy/EnemyCharacter.h"
#include "GeneralSkyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API AGeneralSkyCharacter : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	AGeneralSkyCharacter();

	virtual void BeginPlay() override;
	void StartChargingVisual(float Duration);

	void TickChargeProgress();

	UAnimMontage* GetChargingMontage();
	UAnimMontage* GetStampMontage();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Material")
	UMaterialInstanceDynamic* BodyMat;

	
	UPROPERTY(EditDefaultsOnly,Category="GeneralSkyCharacterAnimation")
	UAnimMontage* ChargeMontage;
	
	UPROPERTY(EditDefaultsOnly,Category="GeneralSkyCharacterAnimation")
	UAnimMontage* StampMontage;

	
	FTimerHandle RedTimer;

private:
	float ChargeVisualTotalTime = 0.f;
	float ChargeVisualStart = -200.f;
	float ChargeVisualEnd = 600.f;
	float ChargeVisualDuration = 2.f;
};
