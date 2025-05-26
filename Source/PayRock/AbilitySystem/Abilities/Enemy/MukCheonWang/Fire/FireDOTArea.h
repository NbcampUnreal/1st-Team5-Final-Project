// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "FireDOTArea.generated.h"

class USphereComponent;
class UNiagaraComponent;

UCLASS()
class PAYROCK_API AFireDOTArea : public AActor
{
	GENERATED_BODY()

public:
	AFireDOTArea();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> AreaCollision;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> VFX;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageInterval = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Area")
	float ExpansionSpeed = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Area")
	float MaxRadius = 400.f;

	UPROPERTY(EditDefaultsOnly, Category = "Lifetime")
	float Lifetime = 3.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageTypeTag;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float Damage = 5.f;

private:
	TSet<AActor*> DamagedActors;
	float CurrentRadius = 0.f;
	float TimeElapsed = 0.f;

	void ApplyDotToOverlappingActors();
};