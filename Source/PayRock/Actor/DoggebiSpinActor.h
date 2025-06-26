// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoggebiSpinActor.generated.h"

UCLASS()
class PAYROCK_API ADoggebiSpinActor : public AActor
{
	GENERATED_BODY()

public:
	ADoggebiSpinActor();
	virtual void Tick(float DeltaTime) override;
	
	void StartAttack();

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TObjectPtr<class UBaseDamageGameplayAbility> SourceDamageAbility;
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TObjectPtr<AActor> SourceActor;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComp;
	
	UPROPERTY(EditDefaultsOnly)
	float AttackSpeed;
	UPROPERTY(EditDefaultsOnly)
	float IdleSpeed;
	UPROPERTY(EditDefaultsOnly)
	float AttackReach;
	
	UPROPERTY()
	FVector BaseBoxExtent;
	UPROPERTY()
	float BaseBoxOffset;
	UPROPERTY(EditDefaultsOnly)
	float BoxExtensionSpeed;

	UPROPERTY()
	FVector BaseMeshScale;
	UPROPERTY(EditDefaultsOnly)
	float MeshScaleMultiplier = .5f;
	
	UPROPERTY(EditDefaultsOnly)
	float Cooldown = 2.f;

private:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void OnRotationComplete();
	void SetCanAttack();

	UPROPERTY()
	TSet<AActor*> AlreadyHitActors;
	
	bool bShouldAttack;
	bool bCanAttack;
	float TotalRotationYaw = 0.f;
	float CurrentExtension = 0.f;

	FTimerHandle CooldownTimer;
};
