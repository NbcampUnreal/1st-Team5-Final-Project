// PayRockGames

#include "PRPlayerController.h"
#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>

APRPlayerController::APRPlayerController()
{
	bReplicates = true;
}

void APRPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(PlayerIMC);
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PlayerIMC, 0);
	}
}

void APRPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(
		MoveAction, ETriggerEvent::Triggered, this, &APRPlayerController::Move);
}

void APRPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxis = InputActionValue.Get<FVector2D>();
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
	const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardVector, InputAxis.Y);
		ControlledPawn->AddMovementInput(RightVector, InputAxis.X);
	}
}