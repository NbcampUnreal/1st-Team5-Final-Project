// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GeometryCache.h"
#include "GeometryCacheComponent.h"
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
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(VisibleAnywhere, Category = "Wave|Collision")
	TObjectPtr<UBoxComponent> CollisionBox;

	FVector MoveDirection;

	UPROPERTY(EditDefaultsOnly, Category = "Wave|Movement")
	float MoveSpeed = 400.f;


	UPROPERTY(EditDefaultsOnly, Category = "Wave|Lifetime")
	float Lifetime = 5.f;


	UPROPERTY(EditDefaultsOnly, Category = "Wave|Knockback")
	float KnockbackStrength = 800.f;

	UPROPERTY(EditDefaultsOnly, Category = "Wave|Effect")
	TObjectPtr<UGeometryCache> GeometryCacheAsset;


	UPROPERTY(VisibleAnywhere, Category = "Wave|Effect")
	TObjectPtr<UGeometryCacheComponent> GeometryCacheComp;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);


	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayVFX();
};