// PayRockGames


#include "GeneralSkyCharacter.h"

#include "NiagaraFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "PayRock/Enemy/EnemyController.h"

AGeneralSkyCharacter::AGeneralSkyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AGeneralSkyCharacter::BeginPlay()
{
	Super::BeginPlay();

	BodyMat = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
}

void AGeneralSkyCharacter::Tick(float DeltaSeconds)
{
	
	Super::Tick(DeltaSeconds);

	if (bIsCharging)
	{
		ChargeVisualTotalTime += DeltaSeconds;
		UpdateChargeProgressVisual();
	}
}

void AGeneralSkyCharacter::UpdateChargeProgressVisual()
{
	if (!BodyMat) return;

	float Alpha = FMath::Clamp(ChargeVisualTotalTime / ChargeVisualDuration, 0.f, 1.f);
	float Interpolated = FMath::Lerp(ChargeVisualStart, ChargeVisualEnd, Alpha);
	BodyMat->SetScalarParameterValue(TEXT("ChargeProgress"), Interpolated);

	if (Alpha >= 1.f)
	{
		bIsCharging = false;
		BodyMat->SetScalarParameterValue(TEXT("ChargeProgress"), ChargeVisualStart);
	}
}

void AGeneralSkyCharacter::NetMulticast_StartChargingVisual_Implementation(float Duration)
{
	if (!BodyMat)
	{
		BodyMat = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
	}
	ChargeVisualDuration = Duration;
	ChargeVisualTotalTime = 0.f;
	bIsCharging = true;
	BodyMat->SetScalarParameterValue(TEXT("ChargeProgress"), ChargeVisualStart);
}

void AGeneralSkyCharacter::NetMulticast_PlayChargeAnim_Implementation()
{
	if (ChargeMontage)
	{
		PlayAnimMontage(ChargeMontage);
	}
}

void AGeneralSkyCharacter::NetMulticast_PlayStampAnim_Implementation()
{
	if (StampMontage)
	{
		PlayAnimMontage(StampMontage);
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

void AGeneralSkyCharacter::Multicast_PlayStampEffect_Implementation(FVector SpawnLocation)
{
	if (StampVFX) // Niagara System
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			StampVFX,
			SpawnLocation,
			FRotator::ZeroRotator,
			FVector(2.f, 2.f, 2.f)
		);
	}
}

void AGeneralSkyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGeneralSkyCharacter, bIsCharging);
}
