// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "FireDOTArea.generated.h"


UCLASS()
class PAYROCK_API AFireDOTArea : public AActor
{
	GENERATED_BODY()

public:
	AFireDOTArea();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void ApplyDotToOverlappingActors();
	void ApplyEffectToActor(AActor* Actor);

	UFUNCTION()
	void OnActorEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> AreaCollision;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> VFX;

	UPROPERTY(EditDefaultsOnly, Category = "DOT")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "DOT")
	FGameplayTag DamageTypeTag;

	UPROPERTY(EditDefaultsOnly, Category = "DOT")
	float Damage = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "DOT")
	float MaxRadius = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "DOT")
	float ExpansionSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "DOT")
	float DamageInterval = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "DOT")
	float Lifetime = 20.f;

	float TimeElapsed = 0.f;
	float CurrentRadius = 0.f;

	UPROPERTY()
	TSet<AActor*> DamagedActors;

	FTimerHandle DamageStartTimerHandle;
};