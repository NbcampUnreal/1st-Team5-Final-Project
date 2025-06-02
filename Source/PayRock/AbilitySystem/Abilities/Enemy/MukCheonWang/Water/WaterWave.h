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

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraEffect;

	UPROPERTY(EditDefaultsOnly)
	float MoveSpeed = 400.f;

	UPROPERTY(EditDefaultsOnly)
	float Lifetime = 5.f;

	UPROPERTY(EditDefaultsOnly)
	float KnockbackStrength = 800.f;

	FVector MoveDirection;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayVFX();
};