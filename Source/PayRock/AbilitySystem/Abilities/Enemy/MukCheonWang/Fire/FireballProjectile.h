// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
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

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UNiagaraComponent> NiagaraComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	TObjectPtr<UNiagaraSystem> ImpactVFX;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageTag;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount = 50.f;

	
	UPROPERTY(EditDefaultsOnly, Category = "DOT")
	TSubclassOf<AFireDOTArea> DOTAreaClass;

	UPROPERTY(EditDefaultsOnly, Category = "DOT")
	float SpawnHeightOffset = 50.f;

	
	UPROPERTY(EditDefaultsOnly, Category = "Timer")
	float FireRate = 1.0f;

	
	float FloatElapsedTime = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "FloatEffect")
	float FloatDuration = 1.5f;

	UPROPERTY(EditDefaultsOnly, Category = "FloatEffect")
	float FloatHeight = 60.f;

	UPROPERTY(EditDefaultsOnly, Category = "FloatEffect")
	float FloatSpeed = 6.f;
	
	
	UPROPERTY()
	FVector LaunchVelocity;

	UPROPERTY()
	bool bHit = false;

	UPROPERTY()
	FTimerHandle LaunchDelayHandle;

	
	UFUNCTION()
	void LaunchToTargetPlayer();

	UFUNCTION()
	void HandleImpact(bool bSpawnDOT);

	UFUNCTION()
	void EnableReplication();
	void PlayImpactVFX();


	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
			   UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
