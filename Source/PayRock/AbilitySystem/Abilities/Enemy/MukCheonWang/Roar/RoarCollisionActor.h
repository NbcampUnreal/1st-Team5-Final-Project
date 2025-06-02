
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

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> RoarEffect;

	UPROPERTY(EditDefaultsOnly)
	float MaxRadius = 1500.f;

	UPROPERTY(EditDefaultsOnly)
	float ExpansionDuration = 1.0f;

	UPROPERTY(EditDefaultsOnly)
	float KnockbackStrength = 1000.f;

	float ElapsedTime = 0.f;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayRoarEffect();
};
