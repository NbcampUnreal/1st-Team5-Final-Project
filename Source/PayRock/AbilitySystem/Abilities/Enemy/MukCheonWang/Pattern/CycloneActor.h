// CycloneActor.h

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/Enemy/MukCheonWang/BaseCombatEffectActor.h"
#include "CycloneActor.generated.h"

class USphereComponent;
class UGeometryCacheComponent;
class UGeometryCache;
class APRCharacter;
class ALightningStrikeActor;

DECLARE_DELEGATE(FOnCycloneDestroyedSignature)


UCLASS()
class PAYROCK_API ACycloneActor : public ABaseCombatEffectActor
{
	GENERATED_BODY()

public:
	ACycloneActor();
	virtual void InitializeEffectSource(UGameplayAbility* InAbility) override;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void Multicast_PlayVFX_Implementation() override;

	void SpawnLightning();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	TObjectPtr<USceneComponent> RootScene;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USphereComponent> PullRange;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UGeometryCacheComponent> GeometryCacheComp;

	UPROPERTY(EditDefaultsOnly, Category = "Cyclone")
	TSubclassOf<ALightningStrikeActor> LightningClass;

	UPROPERTY(EditDefaultsOnly, Category = "Cyclone")
	float PullStrength = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Cyclone")
	float LightningRadius = 800.f;

	UPROPERTY(EditDefaultsOnly, Category = "Cyclone")
	float MinInterval = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Cyclone")
	float MaxInterval = 2.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Cyclone")
	TObjectPtr<UGeometryCache> GeometryCacheAsset;

private:
	FTimerHandle LightningLoopTimer;

	UPROPERTY()
	TArray<APRCharacter*> OverlappingPlayers;
	TWeakObjectPtr<UGameplayAbility> InstigatorAbility;
public:
	FOnCycloneDestroyedSignature OnCycloneDestroyed;
};
