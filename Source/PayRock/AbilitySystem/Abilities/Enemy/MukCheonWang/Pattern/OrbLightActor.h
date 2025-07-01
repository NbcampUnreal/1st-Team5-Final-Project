#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "PayRock/AbilitySystem/Abilities/Enemy/MukCheonWang/BaseCombatEffectActor.h"
#include "OrbLightActor.generated.h"

class UPointLightComponent;
class APRCharacter;

UCLASS()
class PAYROCK_API AOrbLightActor : public ABaseCombatEffectActor
{
	GENERATED_BODY()

public:
	AOrbLightActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Multicast_PlayVFX_Implementation() override;

	void ApplyLightSurvivalDOT();
	void MoveToRandomLocation();

	bool IsPlayerInNavAndOutOfRange(APRCharacter* Player);
	float GetCurrentSpeed() const;
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayRingVFX();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	TObjectPtr<USceneComponent> RootScene;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	TObjectPtr<USceneComponent> FixedScene;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPointLightComponent> LightSource;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	TObjectPtr<USphereComponent> OuterDamageSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	TObjectPtr<USphereComponent> InnerSafeSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UNiagaraComponent> Ring;

	UPROPERTY(EditAnywhere, Category = "Movement")
	TObjectPtr<UBoxComponent> MovementBox;

	UPROPERTY(EditDefaultsOnly)
	float OrbLifetime = 10.f;

	UPROPERTY(EditDefaultsOnly)
	float DamageTickInterval = 1.f;

	UPROPERTY(EditDefaultsOnly)
	float MoveInterval = 2.f;

	UPROPERTY(EditDefaultsOnly)
	float MoveRadius = 3500.f;

	UPROPERTY(EditDefaultsOnly)
	float RequiredProximity = 800.f;

	UPROPERTY(EditDefaultsOnly)
	float MaxSpeed = 500.f;

	UPROPERTY(EditDefaultsOnly)
	float MinSpeed = 150.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float FixedHeight = 150.f;

private:
	TArray<APRCharacter*> CachedPlayers;
	FVector NextTargetLocation;
	float ElapsedTime = 0.f;
	bool bIsMoving = false;

	FTimerHandle DamageTimerHandle;
	FTimerHandle MoveTimerHandle;
	FTimerHandle RingVFXTimerHandle;
};
