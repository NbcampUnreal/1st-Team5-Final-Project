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