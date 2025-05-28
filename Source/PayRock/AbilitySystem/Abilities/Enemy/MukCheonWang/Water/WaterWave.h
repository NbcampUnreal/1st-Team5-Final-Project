// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "WaterWave.generated.h"

UCLASS()
class PAYROCK_API AWaterWave : public AActor
{
	GENERATED_BODY()

public:
	AWaterWave();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Wave")
	float MoveSpeed = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Wave")
	float KnockbackStrength = 1200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Wave")
	float Lifetime = 3.0f;

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						const FHitResult& SweepResult);

	FVector MoveDirection;
};