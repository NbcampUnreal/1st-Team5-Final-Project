// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "StoneSpikeActor.generated.h"

UCLASS()
class PAYROCK_API AStoneSpikeActor : public AActor
{
	GENERATED_BODY()

public:
	AStoneSpikeActor();
	virtual void Tick(float DeltaTime) override;

	void SetInstigatorAbility(UBaseDamageGameplayAbility* InAbility);
	void SetTarget(AActor* InTarget);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> Collision;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> VFX;

	UPROPERTY(EditDefaultsOnly, Category = "Stone")
	TObjectPtr<UCurveFloat> MoveCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Stone")
	float TravelDistance = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Stone")
	float TravelDuration = 2.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageTypeTag;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float Damage = 10.f;

private:
	FVector StartLocation;
	FVector EndLocation;

	FTimerHandle DestroyHandle;

	UPROPERTY()
	TObjectPtr<UBaseDamageGameplayAbility> InstigatorAbility;

	UPROPERTY()
	TObjectPtr<UTimelineComponent> MoveTimeline;

	UPROPERTY()
	TObjectPtr<AActor> TargetActor;

	UFUNCTION()
	void OnTimelineUpdate(float Value);

	UFUNCTION()
	void OnSpikeOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						const FHitResult& SweepResult);
};