// PayRockGames

#include "PRPlayerController.h"
#include "PRPlayerState.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>

APRPlayerController::APRPlayerController()
{
	bReplicates = true;
	DeathOptionsWidgetClass = nullptr;
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

void APRPlayerController::Client_ShowDeathOptions_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Display Death UI"))
/*
	if (DeathOptionsWidgetClass && !DeathOptionsWidget)
	{
		DeathOptionsWidget = CreateWidget<UUserWidget>(this, DeathOptionsWidgetClass);
		if (DeathOptionsWidget)
		{
			DeathOptionsWidget->AddToViewport();
			SetShowMouseCursor(true);
			SetInputMode(FInputModeUIOnly());
		}
	}
*/
}

void APRPlayerController::StartSpectating()
{
	SpectateTargets.Empty();

	for (TActorIterator<ACharacter> It(GetWorld()); It; ++It)
	{
		ACharacter* Chara = *It;
		if (!Chara->IsPlayerControlled()) continue;

		APlayerState* PS = Chara->GetPlayerState();
		if (IsValid(PS))
		{
			APRPlayerState* PRPS = Cast<APRPlayerState>(PS);
			if (PRPS && !PRPS->GetIsDead() && !PRPS->GetIsExtracted())
			{
				SpectateTargets.Add(Chara);
			}
		}
	}
}

void APRPlayerController::SpectateNext()
{
	if (SpectateTargets.Num() == 0) return;

	CurrentSpectateIndex = (CurrentSpectateIndex + 1) % SpectateTargets.Num();
	SetSpectateTarget(SpectateTargets[CurrentSpectateIndex]);
}

void APRPlayerController::SpectatePrevious()
{
	if (SpectateTargets.Num() == 0) return;

	CurrentSpectateIndex = (CurrentSpectateIndex - 1 + SpectateTargets.Num()) % SpectateTargets.Num();
	SetSpectateTarget(SpectateTargets[CurrentSpectateIndex]);
}

void APRPlayerController::SetSpectateTarget(AActor* NewTarget)
{
	if (NewTarget)
	{
		SetViewTargetWithBlend(NewTarget, 0.5f);
		UE_LOG(LogTemp, Log, TEXT("Set view target: %s"), *NewTarget->GetName());
	}
}

