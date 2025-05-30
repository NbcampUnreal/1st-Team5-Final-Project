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
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USphereComponent> DetectionSphere;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UNiagaraComponent> LightVFX;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UPointLightComponent> LightSource;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageTypeTag;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float Damage = 2.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageTickInterval = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Detection")
	float RequiredProximity = 600.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MoveRadius = 1500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MoveInterval = 2.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MinSpeed = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MaxSpeed = 800.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float OrbLifetime = 15.f;

	UPROPERTY()
	TArray<TObjectPtr<APRCharacter>> CachedPlayers;

	UPROPERTY()
	TSet<TObjectPtr<AActor>> DamagedActors;

	FTimerHandle DamageTimerHandle;
	FTimerHandle MoveTimerHandle;

	FVector NextTargetLocation;
	float ElapsedTime = 0.f;
	bool bIsMoving = false;
	UFUNCTION()
	void ApplyLightSurvivalDOT();
	UFUNCTION()
	void ApplyEffectToActor(AActor* Actor);
	UFUNCTION()
	bool IsPlayerInNavAndOutOfRange(APRCharacter* Player);
	UFUNCTION()
	void MoveToRandomLocation();
	
	float GetCurrentSpeed() const;

};