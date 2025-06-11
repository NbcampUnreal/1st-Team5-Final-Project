#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "PayRock/AbilitySystem/Abilities/Enemy/MukCheonWang/BaseCombatEffectActor.h"
#include "FireDOTArea.generated.h"

UCLASS()
class PAYROCK_API AFireDOTArea : public ABaseCombatEffectActor
{
	GENERATED_BODY()

public:
	AFireDOTArea();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnEffectOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
								 bool bFromSweep, const FHitResult& SweepResult) override;

	void ApplyDotToOverlappingActors();
	void ApplyEffectToActor(AActor* Actor);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetVFXScale(float Scale);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "DOT")
	float MaxRadius = 400.f;

	UPROPERTY(EditDefaultsOnly, Category = "DOT")
	float ExpansionSpeed = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "DOT")
	float DamageInterval = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "DOT")
	float InitialRadius = 100.f;
	UPROPERTY(EditDefaultsOnly, Category = "DOT")
	float InitialHalfHeight = 100.f;
	UPROPERTY(EditDefaultsOnly, Category = "DOT")
	float MaxHalfHeight = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* CapsuleComponent;
private:
	TSet<TObjectPtr<AActor>> DamagedActors;
	float CurrentRadius = 0.f;
	float CurrentHeight = 0.f;
	float TimeElapsed = 0.f;

	FTimerHandle DamageStartTimerHandle;
};
