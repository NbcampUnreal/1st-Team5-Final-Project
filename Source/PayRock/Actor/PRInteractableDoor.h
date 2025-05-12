// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PayRock/Interface/PRInterface.h" // 인터페이스 include
#include "PRInteractableDoor.generated.h"

UCLASS()
class PAYROCK_API AInteractableDoor : public AActor, public IPRInterface
{
	GENERATED_BODY()

public:
	AInteractableDoor();

	virtual void Interact(class APRCharacter* Interactor) override;
	virtual FText GetInteractionName() const override;
	virtual bool CanInteract(class APRCharacter* Interactor) const override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Door")
	float OpenAngle = 90.f;

	UPROPERTY(EditAnywhere, Category = "Door")
	float RotationSpeed = 90.f; // 초당 회전 속도

	bool bIsOpening = false;
	bool bIsClosing = false;
	FRotator TargetRotation;

	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	bool bIsOpen = false;
};
