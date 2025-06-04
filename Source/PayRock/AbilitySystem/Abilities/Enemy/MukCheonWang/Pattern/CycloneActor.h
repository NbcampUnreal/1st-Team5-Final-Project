#pragma once

#include "CoreMinimal.h"
#include "GeometryCacheComponent.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "PayRock/AbilitySystem/Abilities/Enemy/MukCheonWang/Lightning/LightningStrikeActor.h"
#include "PayRock/Character/PRCharacter.h"
#include "CycloneActor.generated.h"


UCLASS()
class PAYROCK_API ACycloneActor : public AActor
{
	GENERATED_BODY()

public:
	ACycloneActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> PullRange;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGeometryCacheComponent> GeometryCacheComp;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UGeometryCache> GeometryCacheAsset;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ALightningStrikeActor> LightningClass;

	UPROPERTY(EditDefaultsOnly)
	float PullStrength = 400.f;

	UPROPERTY(EditDefaultsOnly)
	float LightningRadius = 800.f;

	UPROPERTY(EditDefaultsOnly)
	float MinInterval = 1.5f;

	UPROPERTY(EditDefaultsOnly)
	float MaxInterval = 2.5f;

	FTimerHandle LightningLoopTimer;
	TArray<APRCharacter*> OverlappingPlayers;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SpawnLightning();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ActivateCycloneVFX();

public:
	TFunction<void()> OnCycloneDestroyed;
};