// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "StoneSpikeActor.generated.h"

UCLASS()
class PAYROCK_API AStoneSpikeActor : public AActor
{
	GENERATED_BODY()

public:
	AStoneSpikeActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* MovementComp;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> Collision;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> VFX;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DamageTypeTag;

	UPROPERTY(EditDefaultsOnly)
	float Damage = 8.f;

	UPROPERTY(EditDefaultsOnly)
	float BaseSpeed = 500.f;

	UPROPERTY()
	AActor* TargetActor;

	UPROPERTY()
	UBaseDamageGameplayAbility* InstigatorAbility;

	UFUNCTION()
	void OnSpikeOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlaySpikeVFX();

public:
	void SetInstigatorAbility(UBaseDamageGameplayAbility* InAbility);
	
	UFUNCTION()
	void SetTarget(AActor* InTarget);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetTarget(AActor* InTarget);
	void Multicast_SetTarget_Implementation(AActor* InTarget);
};
