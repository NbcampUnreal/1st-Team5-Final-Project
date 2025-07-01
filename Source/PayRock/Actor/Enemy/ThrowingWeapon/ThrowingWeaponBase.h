// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "NiagaraSystem.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ThrowingWeaponBase.generated.h"

UCLASS()
class PAYROCK_API AThrowingWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AThrowingWeaponBase();
	
	UFUNCTION(BlueprintCallable, Category="Throwing")
	virtual void LaunchWeapon(const FVector& LaunchVelocity);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Root;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* Collision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* WeaponMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovement;
	
	UPROPERTY(EditAnywhere, Category="Throwing")
	float LifeAfterHit = 1.0f;
	
	UPROPERTY(EditAnywhere, Category="Throwing")
	UParticleSystem* HitEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FGameplayTag DamageTypeTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FScalableFloat Damage;

	UPROPERTY(BlueprintReadWrite)
	class UBaseDamageGameplayAbility* InstigatorAbility;

	UPROPERTY(EditAnywhere, Category="Movement")
	float MaxTravelDistance = 2000.f;

	UPROPERTY(EditAnywhere, Category="Movement")
	float TravelSpeed = 1000.f;

	FVector InitialLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* TrailEffect;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UNiagaraSystem* TrailEffectTemplate;
	
	void ApplyDamageEffect(AActor* TargetActor);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastActivateTrail(); 

	UFUNCTION()
	void StartTrail();

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;
};