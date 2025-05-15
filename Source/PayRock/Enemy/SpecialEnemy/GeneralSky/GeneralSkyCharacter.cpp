// PayRockGames


#include "GeneralSkyCharacter.h"

#include "PayRock/Enemy/EnemyController.h"

AGeneralSkyCharacter::AGeneralSkyCharacter()
{
	AIControllerClass = AEnemyController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AGeneralSkyCharacter::BeginPlay()
{
	Super::BeginPlay();

	BodyMat = GetMesh()->CreateAndSetMaterialInstanceDynamic(0); 
}

void AGeneralSkyCharacter::StartChargingVisual(float Duration)
{
	
	if (!BodyMat)
	{
		BodyMat = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
	}
	if (!BodyMat) return;

	ChargeVisualStart = -200.f;
	ChargeVisualEnd = 600.f;
	ChargeVisualDuration = Duration;
	ChargeVisualTotalTime = 0.f;

	BodyMat->SetScalarParameterValue(TEXT("ChargeProgress"), ChargeVisualStart);

	const float Step = 0.05f;

	GetWorldTimerManager().SetTimer(RedTimer, this, &AGeneralSkyCharacter::TickChargeProgress, Step, true);
}

void AGeneralSkyCharacter::TickChargeProgress()
{
	ChargeVisualTotalTime += 0.05f;

	float Alpha = FMath::Clamp(ChargeVisualTotalTime / ChargeVisualDuration, 0.f, 1.f);
	float Interpolated = FMath::Lerp(ChargeVisualStart, ChargeVisualEnd, Alpha);

	if (BodyMat)
	{
		BodyMat->SetScalarParameterValue(TEXT("ChargeProgress"), Interpolated);
	}

	if (Alpha >= 1.f)
	{
		GetWorldTimerManager().ClearTimer(RedTimer);
		BodyMat->SetScalarParameterValue(TEXT("ChargeProgress"), ChargeVisualStart); // 리셋
	}
}

UAnimMontage* AGeneralSkyCharacter::GetChargingMontage()
{
	return ChargeMontage;
}

UAnimMontage* AGeneralSkyCharacter::GetStampMontage()
{
	return StampMontage;
}
