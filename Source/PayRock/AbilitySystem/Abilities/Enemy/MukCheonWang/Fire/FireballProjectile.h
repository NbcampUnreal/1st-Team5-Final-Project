#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "PayRock/AbilitySystem/Abilities/Enemy/MukCheonWang/BaseCombatEffectActor.h"
#include "FireballProjectile.generated.h"

class UProjectileMovementComponent;
class AFireDOTArea;
class UNiagaraSystem;

UCLASS()
class PAYROCK_API AFireballProjectile : public ABaseCombatEffectActor
{
	GENERATED_BODY()

public:
	AFireballProjectile();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnEffectOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
								 bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
			   UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void EnableReplication();
	void LaunchToTargetPlayer();
	void HandleImpact(bool bSpawnDOT);

	void PlayImpactVFX();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayImpactVFX();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float FireRate;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float Delay_Min = 0.5;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float Delay_Max = 1.5;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float FloatHeight = 40.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float FloatSpeed = 4.f;

	UPROPERTY(EditDefaultsOnly, Category = "DOT")
	TSubclassOf<AFireDOTArea> DOTAreaClass;

	UPROPERTY(EditDefaultsOnly, Category = "DOT")
	float SpawnHeightOffset = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UNiagaraSystem* ImpactVFX;


	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCapsuleComponent> CapsuleCollision;
private:
	FVector LaunchVelocity;
	float FloatElapsedTime = 0.f;
	bool bHit = false;
	FTimerHandle LaunchDelayHandle;
};
