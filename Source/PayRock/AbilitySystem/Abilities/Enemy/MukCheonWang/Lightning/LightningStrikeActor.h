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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> StrikeCollision;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> LightningVFX;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DamageTypeTag;

	UPROPERTY(EditDefaultsOnly)
	float Damage = 10.f;

	UPROPERTY(EditDefaultsOnly)
	float DelayBeforeStrike = 1.5f;

	FTimerHandle TimerHandle_Activate;

	UPROPERTY()
	TObjectPtr<UBaseDamageGameplayAbility> InstigatorAbility;

	UFUNCTION()
	void OnStrikeOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						 bool bFromSweep, const FHitResult& SweepResult);

	void ActivateStrike();
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayLightningVFX();

public:
	void SetInstigatorAbility(UBaseDamageGameplayAbility* InAbility);
};
