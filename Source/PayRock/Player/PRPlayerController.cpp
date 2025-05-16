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

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PayRock/Character/PRCharacter.h"

APRPlayerController::APRPlayerController()
{
	bReplicates = true;
	// DeathOptionsWidgetClass = nullptr;
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
void APRPlayerController::BindingSpector()
{
	if (UEnhancedInputComponent* EIComp = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// 관전 입력 바인딩
		if (SpectateNextAction)
		{
			EIComp->BindAction(SpectateNextAction, ETriggerEvent::Triggered, this, &APRPlayerController::SpectateNext);
		}
		if (SpectatePrevAction)
		{
			EIComp->BindAction(SpectatePrevAction, ETriggerEvent::Triggered, this, &APRPlayerController::SpectatePrevious);
		}
	}
}
void APRPlayerController::Client_ShowDeathOptions_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Display Death UI"))
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
}

void APRPlayerController::StartSpectating()
{
	SpectateTargets.Empty();
	UE_LOG(LogTemp, Warning, TEXT("SetSpectateTarget() called on %s"), *GetNetModeAsString());

	UE_LOG(LogTemp, Log, TEXT("StartSpectating"));
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

	if (SpectateTargets.Num() > 0)
	{
		CurrentSpectateIndex = 0;
		SetSpectateTarget(SpectateTargets[CurrentSpectateIndex]);

		// 입력 모드 전환
		SetInputMode(FInputModeGameOnly());
		SetShowMouseCursor(false);

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
	ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(PlayerIMC);
			BindingSpector();
			Subsystem->AddMappingContext(SpectorIMC,0);
		}
		UE_LOG(LogTemp, Log, TEXT("관전 시작: 대상 %d명"), SpectateTargets.Num());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("관전 대상 없음"));
	}
}

void APRPlayerController::SpectateNext()
{
	if (SpectateTargets.Num() <= 0) return;

	CurrentSpectateIndex = (CurrentSpectateIndex + 1) % SpectateTargets.Num();
	SetSpectateTarget(SpectateTargets[CurrentSpectateIndex]);
}

void APRPlayerController::SpectatePrevious()
{
	if (SpectateTargets.Num() <= 0) return;

	CurrentSpectateIndex = (CurrentSpectateIndex - 1 + SpectateTargets.Num()) % SpectateTargets.Num();
	SetSpectateTarget(SpectateTargets[CurrentSpectateIndex]);
}

void APRPlayerController::SetSpectateTarget(AActor* NewTarget)
{
	if (!NewTarget) return;

	UnPossess();

	if (APRCharacter* TargetCharacter = Cast<APRCharacter>(NewTarget))
	if (TargetCharacter)
	{
		// 컴포넌트 찾기
		UCameraComponent* CameraComp = TargetCharacter->FindComponentByClass<UCameraComponent>();
		USpringArmComponent* SpringArm = TargetCharacter->FindComponentByClass<USpringArmComponent>();
		// 로그 출력
		UE_LOG(LogTemp, Warning, TEXT("SetSpectateTarget() - Target: %s"), *TargetCharacter->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Camera: %s, Active: %d"), *GetNameSafe(CameraComp), CameraComp ? CameraComp->IsActive() : -1);
		UE_LOG(LogTemp, Warning, TEXT("SpringArm: %s"), *GetNameSafe(SpringArm));
		
		// 카메라 강제 활성화
		if (CameraComp)
		{
			CameraComp->SetActive(true);
			CameraComp->Activate(true);
		}

		// ViewTarget 전환
		SetViewTargetWithBlend(TargetCharacter, 0.3f);

		AActor* ViewTarget = GetViewTarget();
		UE_LOG(LogTemp, Warning, TEXT("Current ViewTarget is: %s"), *GetNameSafe(ViewTarget));
	}
	else
	{
		SetViewTargetWithBlend(NewTarget, 0.3f);
	}
	
}

FString APRPlayerController::GetNetModeAsString() const
{
	UWorld* World = GetWorld();
	if (!World) return TEXT("NoWorld");

	switch (World->GetNetMode())
	{
	case NM_Standalone: return TEXT("Standalone");
	case NM_ListenServer: return TEXT("ListenServer");
	case NM_DedicatedServer: return TEXT("DedicatedServer");
	case NM_Client: return TEXT("Client");
	default: return TEXT("Unknown");
	}
}

