// PayRockGames

#include "PRInteractableDoor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

AInteractableDoor::AInteractableDoor()
{
	PrimaryActorTick.bCanEverTick = true;
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	RootComponent = DoorMesh;
}

void AInteractableDoor::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractableDoor::Interact(APRCharacter* Interactor)
{
	bIsOpen = !bIsOpen;

	FRotator CurrentRot = DoorMesh->GetRelativeRotation();
	TargetRotation = bIsOpen ? FRotator(0.f, OpenAngle, 0.f) : FRotator::ZeroRotator;

	bIsOpening = bIsOpen;
	bIsClosing = !bIsOpen;

	UE_LOG(LogTemp, Log, TEXT("Door %s"), bIsOpen ? TEXT("Opening") : TEXT("Closing"));
}

void AInteractableDoor::Tick(float DeltaTime)
{
	if (!bIsOpening && !bIsClosing) return;

	FRotator CurrentRot = DoorMesh->GetRelativeRotation();
	FRotator NewRot = FMath::RInterpConstantTo(CurrentRot, TargetRotation, DeltaTime, RotationSpeed);

	DoorMesh->SetRelativeRotation(NewRot);

	if (NewRot.Equals(TargetRotation, 0.5f))
	{
		bIsOpening = false;
		bIsClosing = false;
	}
}

FText AInteractableDoor::GetInteractionName() const
{
	return bIsOpen ? FText::FromString("Close Door") : FText::FromString("Open Door");
}

bool AInteractableDoor::CanInteract(APRCharacter* Interactor) const
{
	return true; // 필요시 거리, 조건 추가
}

