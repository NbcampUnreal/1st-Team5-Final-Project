
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseDecalActor.generated.h"

UCLASS()
class PAYROCK_API ABaseDecalActor : public AActor
{
	GENERATED_BODY()

public:
	ABaseDecalActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDecalComponent> Decal;

	UPROPERTY()
	TObjectPtr<AActor> FollowTarget;

	FVector InitialLocation;
	FVector Direction;
	
	UPROPERTY(EditDefaultsOnly)
	float LifeDuration = 1.5f;

	UPROPERTY(EditDefaultsOnly)
	float MaxScale = 2.0f;

	UPROPERTY(EditDefaultsOnly)
	float OffsetSpeed = 50.f;

	UPROPERTY(EditDefaultsOnly)
	float ScaleSpeed = 1.f;

	float TimeElapsed = 0.f;

public:
	void Init(AActor* Target);
};