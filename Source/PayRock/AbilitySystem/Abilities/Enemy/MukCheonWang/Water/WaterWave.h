#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/Enemy/MukCheonWang/BaseCombatEffectActor.h"
#include "WaterWave.generated.h"

class UBoxComponent;
class UGeometryCacheComponent;
class UGeometryCache;
class APRCharacter;

UCLASS()
class PAYROCK_API AWaterWave : public ABaseCombatEffectActor
{
	GENERATED_BODY()
	
public:
	AWaterWave();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnEffectOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
								 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
								 bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void Multicast_PlayVFX_Implementation() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UGeometryCacheComponent> GeometryCacheComp;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	TObjectPtr<UGeometryCache> GeometryCacheAsset;

	UPROPERTY(EditDefaultsOnly, Category = "Knockback")
	float KnockbackStrength = 1500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MoveSpeed = 600.f;

	UPROPERTY(EditDefaultsOnly, Category = "Lifetime")
	float Lifetime = 4.f;

private:
	FVector MoveDirection;
};
