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
#include "Kismet/GameplayStatics.h"
#include "PayRock/GameSystem/PRAdvancedGameInstance.h"
#include "PayRock/GameSystem/PRGameMode.h"
#include "PayRock/UI/Manager/UIManager.h"
#include "PayRock/UI/Widget/CleanPercent.h"
#include "PayRock/UI/Widget/InGameWidget.h"

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
		// 한번 눌렀을 때만 발동 하게! esc
		EIComp->BindAction(ToggleMenuAction, ETriggerEvent::Started, this, &APRPlayerController::ToggleSettingsMenu);

		// H눌렀을때 Help  바인딩 
		EIComp->BindAction(ShowHelpAction, ETriggerEvent::Started, this, &APRPlayerController::ToggleHelpMenu);
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
	if (UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>())
	{
		UIManager->RemoveAllWidgets();
		UIManager->RemoveAllWidgetControllers();
		UIManager->ShowWidget(EWidgetCategory::LeaveOptions);
		SetShowMouseCursor(true);
		SetInputMode(FInputModeUIOnly());
	}
}

void APRPlayerController::RemoveDeathOptions()
{
	if (IsLocalController())
	{
		if (UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>())
		{
			UIManager->RemoveWidget(EWidgetCategory::LeaveOptions);
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
		SetInputMode(FInputModeGameAndUI());

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

	// UnPossess();

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

void APRPlayerController::Client_NotifyQuestKill_Implementation(AEnemyCharacter* DeadEnemy)
{
	if (UPRAdvancedGameInstance* PRGI = Cast<UPRAdvancedGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		UE_LOG(LogTemp, Log, TEXT("[APRPlayerController]:UPRAdvancedGameInstance불러오기성공")  );
		const FString TargetName = PRGI->GetQuestManager()->GetCurrentQuest().TargetName;

		if (AEnemyCharacter* DeadCharacter = DeadEnemy)
		{
			UE_LOG(LogTemp, Log, TEXT("[APRPlayerController]:AEnemyCharacter 불러오기성공")  );
			const FName CharacterTypeName = StaticEnum<ECharacterType>()->GetNameByValue((int64)DeadCharacter->CharacterType);
			FString CharacterTypeString = CharacterTypeName.ToString();
			FString ShortName;
			CharacterTypeString.Split(TEXT("::"), nullptr, &ShortName);

			if (TargetName == ShortName)
			{
				PRGI->GetQuestManager()->UpdateProgress();
				UE_LOG(LogTemp, Log, TEXT("[APRPlayerController] 퀘스트 진행도 증가: %s"), *ShortName);
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
		EWidgetCategory::Help,
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

void APRPlayerController::ToggleHelpMenu()
{
	UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
	if (!UIManager) return;

	UUserWidget* HelpWidget = UIManager->FindWidget(EWidgetCategory::Help);

	// ; H 눌렀을 때 마우스 보이게
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
	SetShowMouseCursor(true);

	if (HelpWidget && HelpWidget->IsInViewport())
	{

		//=== ; 헬프UI 있는 경우에 H 누르면 다시 마우스 안보이게 
		SetInputMode(FInputModeGameOnly());
		SetShowMouseCursor(false);
		bIsSettingsMenuOpen = false;
		//===
		UIManager->RemoveWidget(EWidgetCategory::Help);
	}
	else
	{
		UIManager->ShowWidget(EWidgetCategory::Help);
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
		UIManager->RemoveAllWidgets();
		UIManager->RemoveAllWidgetControllers();
		UIManager->ShowWidget(EWidgetCategory::MatchHUD);
		break;
	case EMatchFlowState::MatchInProgress:
		UIManager->RemoveAllWidgets();
		UIManager->RemoveAllWidgetControllers();
		UIManager->ShowWidget(EWidgetCategory::InGameHUD);
		UpdateClothesColor();
		UpdateCleanData();
		break;
	case EMatchFlowState::ExtractionEnabled:
		OnExtractionEnabled.Broadcast();
		break;
	case EMatchFlowState::MatchEnded:
		SetInputMode(FInputModeUIOnly());
		DisableInput(this);
		UIManager->RemoveAllWidgets();
		UIManager->RemoveAllWidgetControllers();
		UIManager->ShowWidget(EWidgetCategory::MatchEnd);
		break;
	}
}

void APRPlayerController::UpdateCleanData()
{
	if (IsLocalController())
	{
		UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
		if (!UIManager) return;
		UUserWidget* FoundWidget = UIManager->FindWidget(EWidgetCategory::InGameHUD);
		if (!FoundWidget) return;
		if (UInGameWidget* InGameHUD = Cast<UInGameWidget>(FoundWidget))
		{
			InGameHUD->UpdateCleanPercent();
		}
	}
}

void APRPlayerController::UpdateClothesColor()
{
	if (APRCharacter* PRCharacter = Cast<APRCharacter>(GetCharacter()))
	{
		PRCharacter->UpdateClothesColor();
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
		FVector Location = IsValid(GetCharacter()) ? GetCharacter()->GetActorLocation() : FVector(0, 0, 0);
		GameMode->SpawnAndPossessNecroCharacter(this, Location);
	}

	ClientOnNecroPossessed();
}

void APRPlayerController::ClientOnNecroPossessed_Implementation()
{
	if (IsLocalController())
	{
		SetInputMode(FInputModeGameOnly());
		EnableInput(this);
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(SpectorIMC);
			Subsystem->RemoveMappingContext(PlayerIMC);
			Subsystem->AddMappingContext(PlayerIMC,0);
		}
		SetShowMouseCursor(false);
	}
}

