
#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "RoarCollisionActor.generated.h"

UCLASS()
class PAYROCK_API ARoarCollisionActor : public AActor
{
	GENERATED_BODY()

public:	
	ARoarCollisionActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	TObjectPtr<USphereComponent> CollisionSphere;
	
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	TObjectPtr<UNiagaraSystem> RoarEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Roar")
	float MaxRadius = 1200.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Roar")
	float ExpansionDuration = 1.5f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Roar")
	float KnockbackStrength = 1000.f;
	
	float ElapsedTime = 0.f;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};