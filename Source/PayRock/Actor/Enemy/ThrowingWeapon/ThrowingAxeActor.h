// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "ThrowingWeaponBase.h"
#include "ThrowingAxeActor.generated.h"

UCLASS()
class PAYROCK_API AThrowingAxeActor : public AThrowingWeaponBase
{
	GENERATED_BODY()

public:
	AThrowingAxeActor();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="Throwing|Visual")
	FRotator RotationSpeed = FRotator(0.f, 1500.f, 0.f);

	
};
