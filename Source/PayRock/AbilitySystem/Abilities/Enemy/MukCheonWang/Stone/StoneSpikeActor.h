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
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnSpikeOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						const FHitResult& SweepResult);

public:
	void SetInstigatorAbility(UBaseDamageGameplayAbility* InAbility);
	void SetTarget(AActor* InTarget);

private:
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* Collision;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* VFX;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	UCurveFloat* SpeedCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float TravelDistance = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float TravelDuration = 2.0f;

	float ElapsedTime = 0.f;

	TObjectPtr<AActor> TargetActor;
	TObjectPtr<UBaseDamageGameplayAbility> InstigatorAbility;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageTypeTag;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float Damage = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float BaseSpeed = 500.f;
};