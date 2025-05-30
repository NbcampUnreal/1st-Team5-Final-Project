#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "PayRock/AbilitySystem/Abilities/Enemy/MukCheonWang/Lightning/LightningStrikeActor.h"
#include "PayRock/Character/PRCharacter.h"
#include "CycloneActor.generated.h"


UCLASS()
class PAYROCK_API ACycloneActor : public AActor
{
	GENERATED_BODY()

public:
	ACycloneActor();
	
	TFunction<void()> OnCycloneDestroyed;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(VisibleAnywhere, Category = "VFX")
	TObjectPtr<UNiagaraComponent> CycloneVFX;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	TObjectPtr<USphereComponent> PullRange;

	UPROPERTY(EditDefaultsOnly, Category = "Lightning")
	TSubclassOf<ALightningStrikeActor> LightningClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Timing")
	float LifeTime = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Physics")
	float PullStrength = 1200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Lightning")
	float LightningRadius = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Lightning")
	float MinInterval = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "Lightning")
	float MaxInterval = 0.3f;
	
	TArray<APRCharacter*> OverlappingPlayers;
	
	FTimerHandle LightningLoopTimer;
	FTimerHandle DestroyTimerHandle;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void SpawnLightning();
};