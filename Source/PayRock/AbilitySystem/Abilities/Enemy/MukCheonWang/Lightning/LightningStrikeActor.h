// LightningStrikeActor.h
#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/Enemy/MukCheonWang/BaseCombatEffectActor.h"
#include "LightningStrikeActor.generated.h"

class UCapsuleComponent;

UCLASS()
class PAYROCK_API ALightningStrikeActor : public ABaseCombatEffectActor
{
	GENERATED_BODY()

public:
	ALightningStrikeActor();

protected:
	virtual void BeginPlay() override;
	virtual void OnEffectOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
								 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
								 bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	void ActivateStrike();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayLightningVFX();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCapsuleComponent> StrikeCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UNiagaraComponent> LightningVFX;

	UPROPERTY(EditDefaultsOnly, Category = "Timing")
	float DelayBeforeStrike = 1.5f;

private:
	FTimerHandle TimerHandle_Activate;
};
