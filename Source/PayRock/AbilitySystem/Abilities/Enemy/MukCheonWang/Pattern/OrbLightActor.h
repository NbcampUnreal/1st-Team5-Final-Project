#pragma once

#include "CoreMinimal.h"
#include "NavigationSystem.h"
#include "NiagaraComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "PayRock/Character/PRCharacter.h"
#include "OrbLightActor.generated.h"

UCLASS()
class PAYROCK_API AOrbLightActor : public AActor
{
	GENERATED_BODY()

public:
	AOrbLightActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> DetectionSphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> LightVFX;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPointLightComponent> LightSource;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DamageTypeTag;

	UPROPERTY(EditDefaultsOnly)
	float Damage = 2.5f;

	UPROPERTY(EditDefaultsOnly)
	float RequiredProximity = 800.f;

	UPROPERTY(EditDefaultsOnly)
	float DamageTickInterval = 1.0f;

	UPROPERTY(EditDefaultsOnly)
	float MoveInterval = 3.0f;

	UPROPERTY(EditDefaultsOnly)
	float MoveRadius = 2000.f;

	UPROPERTY(EditDefaultsOnly)
	float OrbLifetime = 20.0f;

	UPROPERTY(EditDefaultsOnly)
	float MinSpeed = 200.f;

	UPROPERTY(EditDefaultsOnly)
	float MaxSpeed = 800.f;

	TArray<APRCharacter*> CachedPlayers;
	TSet<AActor*> DamagedActors;
	FTimerHandle DamageTimerHandle;
	FTimerHandle MoveTimerHandle;

	bool bIsMoving = false;
	float ElapsedTime = 0.f;
	FVector NextTargetLocation;

	UFUNCTION()
	void ApplyLightSurvivalDOT();

	UFUNCTION()
	void MoveToRandomLocation();

	void ApplyEffectToActor(AActor* Actor);
	bool IsPlayerInNavAndOutOfRange(APRCharacter* Player);
	float GetCurrentSpeed() const;
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ActivateVisuals();
};
