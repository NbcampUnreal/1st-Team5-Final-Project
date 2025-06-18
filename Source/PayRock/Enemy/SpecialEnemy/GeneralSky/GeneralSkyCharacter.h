// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Enemy/EnemyCharacter.h"
#include "NiagaraSystem.h"
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
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_StartChargingVisual(float Duration);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_PlayChargeAnim();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_PlayStampAnim();

	UAnimMontage* GetChargingMontage();
	UAnimMontage* GetStampMontage();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayStampEffect(FVector SpawnLocation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	TObjectPtr<UNiagaraSystem> StampVFX;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterialInstanceDynamic* BodyMat;

	UPROPERTY(EditDefaultsOnly, Category = "GeneralSkyCharacterAnimation")
	UAnimMontage* ChargeMontage;

	UPROPERTY(EditDefaultsOnly, Category = "GeneralSkyCharacterAnimation")
	UAnimMontage* StampMontage;

private:
	UPROPERTY(Replicated)
	bool bIsCharging = false;

	float ChargeVisualTotalTime = 0.f;
	float ChargeVisualStart = -200.f;
	float ChargeVisualEnd = 600.f;
	float ChargeVisualDuration = 2.f;

	void UpdateChargeProgressVisual();
};