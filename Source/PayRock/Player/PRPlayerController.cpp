// PayRockGames

#include "PRPlayerController.h"
#include "PRPlayerState.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Character.h"
#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PayRock/Character/PRCharacter.h"
#include "PayRock/UI/Widget/BaseUserWidget.h"
//#include "PayRock/GameSystem/PRGameState.h"
#include "PayRock/GameSystem/PRGameMode.h"
#include "PayRock/UI/Manager/UIManager.h"

APRPlayerController::APRPlayerController()
	: DeathOptionsWidget(nullptr)
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

	if (IsLocalController())
	{
		APRGameState* PRGameState = GetWorld()->GetGameState<APRGameState>();
		if (!IsValid(PRGameState)) return;
        
		// MatchFlowState 강제 적용
		HandleMatchFlowStateChanged(PRGameState->GetMatchFlowState());
	}
}

void APRPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EIComp = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// 한번 눌렀을 때만 발동 하게! 
		EIComp->BindAction(ToggleMenuAction, ETriggerEvent::Started, this, &APRPlayerController::ToggleSettingsMenu);
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

void APRPlayerController::RemoveDeathOptions()
{
	if (DeathOptionsWidget && IsLocalController())
	{
		DeathOptionsWidget->RemoveFromParent();
		DeathOptionsWidget = nullptr;
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

void APRPlayerController::Client_OnSpectateTargetDied_Implementation(AActor* DeadActor)
{
	OnSpectateTargetDied(DeadActor);
}

void APRPlayerController::OnSpectateTargetDied(AActor* DeadActor)
{
	if (SpectateTargets.Contains(DeadActor))
	{
		SpectateTargets.Remove(DeadActor);

		if (DeadActor == GetViewTarget())
		{
			UE_LOG(LogTemp, Warning, TEXT("관전하던 대상이 사망 또는 탈출함. 다음 대상으로 전환"));

			if (SpectateTargets.Num() > 0)
			{
				CurrentSpectateIndex = CurrentSpectateIndex % SpectateTargets.Num();
				SetSpectateTarget(SpectateTargets[CurrentSpectateIndex]);
			}
			else
			{
				SetViewTarget(nullptr);
			}
		}
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



void APRPlayerController::ToggleSettingsMenu()
{
	UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
	if (!UIManager) return;

	// 1. 블루프린트 인벤토리 닫기 (우선순위 가장 높음)
	if (APRCharacter* MyChar = Cast<APRCharacter>(GetCharacter()))
	{
		TArray<UActorComponent*> Components = MyChar->GetComponents().Array();
		for (UActorComponent* Comp : Components)
		{
			if (Comp && Comp->GetName().Contains(TEXT("BP_InventoryComponent")))
			{
				// 먼저 IsInventoryVisible 호출해서 확인
				UFunction* CheckFunc = Comp->FindFunction(FName("IsInventoryVisible?"));
				if (CheckFunc)
				{
					struct
					{
						bool ReturnValue;
					} Params;

					Comp->ProcessEvent(CheckFunc, &Params);

					if (Params.ReturnValue) // 인벤토리가 켜져있다면
					{
						// CloseInventory 호출
						UFunction* CloseFunc = Comp->FindFunction(FName("CloseInventory"));
						if (CloseFunc)
						{
							Comp->ProcessEvent(CloseFunc, nullptr);

							//===
							SetInputMode(FInputModeGameOnly());
							SetShowMouseCursor(false);
							bIsSettingsMenuOpen = false;
							//===
							return; // 하나만 닫고 함수 종료
						}
					}
				}
			}
		}
	}

	// 2. UIManager에서 관리되는 위젯 닫기
	TArray<EWidgetCategory> UICheckList = {
		EWidgetCategory::Stat,
		EWidgetCategory::Stash,
		EWidgetCategory::Blessing,
	};

	for (EWidgetCategory Category : UICheckList)
	{
		if (UUserWidget* Widget = UIManager->FindWidget(Category))
		{
			if (Widget->IsVisible())
			{
				UIManager->HideWidget(Category);

				//===
				SetInputMode(FInputModeGameOnly());
				SetShowMouseCursor(false);
				bIsSettingsMenuOpen = false;
				//===

				return; //  하나만 닫고 종료
			}
		}
	}

	// 3. 아무것도 안 열려 있으면 옵션 메뉴 토글
	if (!SettingsMenuWidgetClass) return;

	if (!bIsSettingsMenuOpen)
	{
		SettingsMenuWidget = CreateWidget<UBaseUserWidget>(this, SettingsMenuWidgetClass);
		if (SettingsMenuWidget)
		{
			SettingsMenuWidget->AddToViewport();

			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetHideCursorDuringCapture(false);
			SetInputMode(InputMode);
			SetShowMouseCursor(true);

			bIsSettingsMenuOpen = true;
		}
	}
	else
	{
		if (SettingsMenuWidget && SettingsMenuWidget->IsInViewport())
		{
			SettingsMenuWidget->RemoveFromParent();
			SettingsMenuWidget = nullptr;
		}
		SetInputMode(FInputModeGameOnly());
		SetShowMouseCursor(false);
		bIsSettingsMenuOpen = false;
	}
}


void APRPlayerController::HandleMatchFlowStateChanged(EMatchFlowState NewState)
{
	// UE_LOG(LogTemp, Error, TEXT("HandleMatchFlowStateChanged: %d"), static_cast<int32>(NewState));
	if (!IsLocalController()) return;

	UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
	if (!UIManager) return;

	switch (NewState)
	{
	case EMatchFlowState::WaitingToStart:
		UIManager->ShowWidget(EWidgetCategory::MatchHUD);
		break;
	case EMatchFlowState::MatchInProgress:
		UIManager->RemoveWidget(EWidgetCategory::MatchHUD);
		UIManager->ShowWidget(EWidgetCategory::InGameHUD);
		break;
	case EMatchFlowState::ExtractionEnabled:
		// 추가 UI 업데이트 원하면 여기에
		break;
	case EMatchFlowState::MatchEnded:
		UIManager->RemoveAllWidgets();
		UIManager->RemoveAllWidgetControllers();
		break;
	}
}

/*
 *	Necro Character
 */
void APRPlayerController::ServerRequestNecroCharacter_Implementation()
{
	if (!HasAuthority()) return;
	if (!GetWorld() || GetWorld()->bIsTearingDown) return;

	if (APRGameMode* GameMode = GetWorld()->GetAuthGameMode<APRGameMode>())
	{
		GameMode->SpawnAndPossessNecroCharacter(this);
	}

	ClientOnNecroPossessed();
}

void APRPlayerController::ClientOnNecroPossessed_Implementation()
{
	if (IsLocalController())
	{
		SetInputMode(FInputModeGameOnly());
		EnableInput(this);
		SetShowMouseCursor(false);
	}
}

