// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "LightningStrikeActor.generated.h"

UCLASS()
class PAYROCK_API ALightningStrikeActor : public AActor
{
	GENERATED_BODY()

public:
	
	ALightningStrikeActor();
	virtual void Tick(float DeltaTime) override;

	void SetInstigatorAbility(UBaseDamageGameplayAbility* InAbility);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> StrikeCollision;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> LightningVFX;

	UPROPERTY(EditDefaultsOnly, Category = "Lightning")
	float DelayBeforeStrike = 1.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Lightning")
	float Damage = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Lightning")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Lightning")
	FGameplayTag DamageTypeTag;

private:
	FTimerHandle TimerHandle_Activate;
	FTimerHandle TimerHandle_Destroy;

	UPROPERTY()
	TObjectPtr<UBaseDamageGameplayAbility> InstigatorAbility;

	UFUNCTION()
	void ActivateStrike();

	UFUNCTION()
	void OnStrikeOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						 bool bFromSweep, const FHitResult& SweepResult);
};