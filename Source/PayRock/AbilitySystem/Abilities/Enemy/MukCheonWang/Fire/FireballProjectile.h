#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "FireballProjectile.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class USphereComponent;
class UProjectileMovementComponent;
class AFireDOTArea;

UCLASS()
class PAYROCK_API AFireballProjectile : public AActor
{
	GENERATED_BODY()

public:
	AFireballProjectile();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageTag;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TObjectPtr<UNiagaraSystem> ImpactVFX;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TSubclassOf<AFireDOTArea> DOTAreaClass;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	float SpawnHeightOffset = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float FireRate = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float FloatSpeed = 4.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float FloatHeight = 15.0f;

private:
	bool bHit = false;
	FVector LaunchVelocity;
	FTimerHandle LaunchDelayHandle;
	float FloatElapsedTime = 0.f;

	void LaunchToTargetPlayer();
	void HandleImpact(bool bSpawnDOT);
	void EnableReplication();

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                     const FHitResult& SweepResult);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	           UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void PlayImpactVFX();
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayImpactVFX();
	void Multicast_PlayImpactVFX_Implementation();
};
