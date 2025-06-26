// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class UGameplayEffect;
class UNiagaraSystem;
class UNiagaraComponent;
class USphereComponent;
class UProjectileMovementComponent;
class USceneComponent;

UCLASS()
class PAYROCK_API ABaseProjectile : public AActor
{
	GENERATED_BODY()

public:
	ABaseProjectile();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TObjectPtr<UGameplayAbility> SourceAbility;

	UPROPERTY(BlueprintReadWrite, Replicated, meta = (ExposeOnSpawn = true))
	TObjectPtr<AActor> SourceActor;
	
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	bool bHit = false;

	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 4.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> SceneRoot;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> Niagara;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> AdditionalEffectToApply;

	UPROPERTY(EditDefaultsOnly)
	float ProjectileSpeed = 550.f;
};
