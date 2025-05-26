// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireballProjectile.generated.h"

class UProjectileMovementComponent;
class UNiagaraComponent;
class USphereComponent;
class AFireDOTArea;

UCLASS()
class PAYROCK_API AFireballProjectile : public AActor
{
	GENERATED_BODY()

public:
	AFireballProjectile();

	virtual void Tick(float DeltaTime) override;

	void InitTarget(AActor* InTarget);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TSubclassOf<AFireDOTArea> DOTAreaClass;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	float SpawnHeightOffset = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	float ImpactDelay = 0.1f;

private:
	UPROPERTY()
	TObjectPtr<AActor> TargetActor;

	void HandleImpact();
};