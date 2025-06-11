#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/Enemy/MukCheonWang/BaseCombatEffectActor.h"
#include "StoneSpikeActor.generated.h"

class UCapsuleComponent;
class UProjectileMovementComponent;

UCLASS()
class PAYROCK_API AStoneSpikeActor : public ABaseCombatEffectActor
{
	GENERATED_BODY()

public:
	AStoneSpikeActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnEffectOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
								 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
								 bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	void DestroySpike();

public:
	void SetTarget(AActor* InTarget);
	void PlayRepeatedSpikeVFX();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetTarget(AActor* InTarget);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlaySpikeVFX();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCapsuleComponent> CapsuleCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> MovementComp;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float BaseSpeed = 500.f;

	UPROPERTY()
	TObjectPtr<AActor> TargetActor;
	
	FTimerHandle VFXRepeatTimerHandle;
	FTimerHandle DestroyDelayHandle;
};
