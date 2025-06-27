#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "BaseCombatEffectActor.generated.h"

class UNiagaraComponent;
class UGameplayEffect;
class UBaseDamageGameplayAbility;
class UPrimitiveComponent;

UCLASS()
class PAYROCK_API ABaseCombatEffectActor : public AActor
{
	GENERATED_BODY()

public:
	ABaseCombatEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnEffectOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
								 bool bFromSweep, const FHitResult& SweepResult);


	UFUNCTION(NetMulticast, Reliable)
	virtual void Multicast_PlayVFX();
	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UPrimitiveComponent> CollisionComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UNiagaraComponent> VFX;

	
	UPROPERTY(EditDefaultsOnly, Category = "Lifetime")
	float DefaultLifeSpan = 5.f;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GAS")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GAS")
	FGameplayTag DamageTypeTag;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GAS")
	float DamageAmount = 10.f;



	UPROPERTY()
	TObjectPtr<UBaseDamageGameplayAbility> InstigatorAbility;

public:
	virtual void InitializeEffectSource(UGameplayAbility* InAbility);
protected:
	void ApplyDamageEffect(AActor* TargetActor);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
