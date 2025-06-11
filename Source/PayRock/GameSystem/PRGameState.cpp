// PayRockGames PRGameState.cpp


#include "PRGameState.h"
#include "PRGameMode.h"
#include "PayRock/Player/PRPlayerState.h"
#include "UObject/UObjectGlobals.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "PayRock/Character/BaseCharacter.h"
#include "PayRock/Player/PRPlayerController.h"

int32 APRGameState::GetMinimumRequirePlayers()
{
	return MinimumRequirePlayers;
}

int32 APRGameState::GetCurrentAmountOfPlayers()
{
	return CurrentAmountOfPlayers;
}

int32 APRGameState::GetMatchStartCountDown()
{
	return MatchStart_CountDown;
}

int32 APRGameState::GetMatchDurationSeconds()
{
	return MatchDurationSeconds;
}

int32 APRGameState::GetExtractionActivationTime()
{
	return ExtractionActivationTime;
}

int32 APRGameState::GetRemainingTime()
{
	return RemainingMatchTime;
}

APRGameState::APRGameState()
{
	MinimumRequirePlayers = 2;  // 매치 시작시 필요한 플레이어 수
	CurrentAmountOfPlayers = 0; // 현재 플레이어 수 초기화
	MatchStart_CountDown = -1;  // 매치 시작 카운트다운
	MatchDurationSeconds = 300; // 매치 시작 후 매치 지속시간
	ExtractionActivationTime = 180; // 탈출구 열리는 시간
	RemainingMatchTime = MatchDurationSeconds;
	bReplicates = true;
}

void APRGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APRGameState, CurrentAmountOfPlayers);
	DOREPLIFETIME(APRGameState, MinimumRequirePlayers);
	DOREPLIFETIME(APRGameState, MatchStart_CountDown);
	DOREPLIFETIME(APRGameState, RemainingMatchTime);

	// 추가;
	DOREPLIFETIME(APRGameState, MatchFlowState); 
	DOREPLIFETIME(APRGameState, RemainingExtractionTime);  // 서버-> 클라로 자동 복사 매크로  

}

void APRGameState::Notify_PlayerConnection_Implementation()
{
	if (!HasAuthority()) return;

	// 최신 접속 인원 수
	CurrentAmountOfPlayers = GetWorld()->GetGameState<AGameState>()->PlayerArray.Num();

	// 60초 강제 시작 타이머가 없다면 생성
	if (!bForceStarted && !GetWorld()->GetTimerManager().IsTimerActive(ForceStartTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(ForceStartTimerHandle, this, &APRGameState::ForceStartMatch, 60.0f, false);
	}

	// 조건 충족 시
	if (CurrentAmountOfPlayers >= MinimumRequirePlayers)
	{
		if (!bHasStartedCountdown)
		{
			// 카운트다운 시작
			bHasStartedCountdown = true;
			MatchStart_CountDown = 30;
			GetWorld()->GetTimerManager().SetTimer(MatchStartTimerHandle, this, &APRGameState::TickMatchCountdown, 1.0f, true);

			//; 대기시간에 플레이어컨트롤러에 알리기
			SetMatchFlowState(EMatchFlowState::WaitingToStart);
		}
		else
		{
			// 이미 카운트다운 중이라면: 남은 시간이 15초보다 크면 15초로 "줄임"
			if (MatchStart_CountDown > 30)
			{
				MatchStart_CountDown = 30;
			}
		}
	}
}


int32 APRGameState::GetAlivePlayerCount() const
{
	int32 AliveCount = 0;
	for (APlayerState* PS : PlayerArray)
	{
		const APRPlayerState* Player = Cast<APRPlayerState>(PS);
		if (!Player) continue;

		const bool bIsDead = Player->GetIsDead();
		const bool bIsExtracted = Player->GetIsExtracted();
		if (!bIsDead && !bIsExtracted) ++AliveCount;
	} 
	return AliveCount;
}


void APRGameState::CheckAlivePlayers()
{
	const int32 AliveCount = GetAlivePlayerCount();
	UE_LOG(LogTemp, Warning, TEXT("Alive Player count: %i"), AliveCount);

	if (AliveCount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("All Players DEAD. End the Match...."));
		MatchEnd();
	}
	
}

void APRGameState::MatchEnd() 
{
	UE_LOG(LogTemp, Warning, TEXT("Match Ended"));
	GetWorld()->GetTimerManager().ClearTimer(AliveCheckTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ExtractionActivationTimerHandle);
	for (APlayerState* PS : PlayerArray)
	{
		APRPlayerState* PRPS = Cast<APRPlayerState>(PS);
		if (!PRPS->GetIsExtracted()) PRPS->ForceDeath();
	}
	GetWorld()->GetTimerManager().SetTimer(
		MatchEndTimerHandle,
		this,
		&APRGameState::CallTheGmToEnd,
		10,
		false
	);
	


	//;
	SetMatchFlowState(EMatchFlowState::MatchEnded); // 여기 추가

}

void APRGameState::TickMatchCountdown()
{
	if (!HasAuthority()) return;

	MatchStart_CountDown--;

	if (MatchStart_CountDown <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(MatchStartTimerHandle);
		StartMatch();
	}
}

void APRGameState::StartMatch()
{
	UE_LOG(LogTemp, Warning, TEXT("MatchStarting... Teleporting Players..."));

	APRGameMode* GM = Cast<APRGameMode>(GetWorld()->GetAuthGameMode());
	if (!GM) return;

	for (APlayerState* PS : PlayerArray)
	{
		if (!PS) continue;

		AController* Controller = Cast<AController>(PS->GetOwner());
		if (!Controller) continue;

		APawn* Pawn = Controller->GetPawn();
		if (!Pawn) continue;

		AActor* StartSpot = GM->ChooseMatchStartSpot();
		if (!StartSpot) continue;
		
		// 순간이동
		Pawn->SetActorLocationAndRotation(
			StartSpot->GetActorLocation(),
			StartSpot->GetActorRotation(),
			false, // sweep
			nullptr,
			ETeleportType::TeleportPhysics
		);
	}
	RemainingMatchTime = MatchDurationSeconds;

	GetWorld()->GetTimerManager().SetTimer( // 매치 제한시간 로직
		MatchTimerHandle,
		this,
		&APRGameState::MatchEnd,
		MatchDurationSeconds,
		false
	);
	GetWorld()->GetTimerManager().SetTimer( // 매치 진행시간 타이머 (남은시간)
	MatchTimerHandle,
	this,
	&APRGameState::TickMatchTimer,
	1.0f,
	true
	);
	GetWorld()->GetTimerManager().SetTimer( // 탈출 포인트 열림 로직
	ExtractionActivationTimerHandle,
	this,
	&APRGameState::EnableExtractionZones,
	ExtractionActivationTime,
	false
	);
	GetWorld()->GetTimerManager().SetTimer( // 탈출구 열리는 타이머 등록
		ExtractionCountDownTimerHandle,
		this,
		&APRGameState::TickExtractionTimer,
		1.0f,
		true
	);
	GetWorld()->GetTimerManager().SetTimer( // 남은 플레이어수 추적
		AliveCheckTimerHandle,
		this,
		&APRGameState::CheckAlivePlayers,
		2.f,
		true
	);

	// ; 탈출구 열리는 시간 받아오기 !!
	RemainingExtractionTime = ExtractionActivationTime;


	UE_LOG(LogTemp, Warning, TEXT("Match timer started. Duration: %d seconds"), MatchDurationSeconds);


	SetMatchFlowState(EMatchFlowState::MatchInProgress); // ;여기 추가

}


void APRGameState::ForceStartMatch()
{
	if (!bHasStartedCountdown)
	{
		bHasStartedCountdown = true;
		bForceStarted = true;
		MatchStart_CountDown = 30;
		GetWorld()->GetTimerManager().SetTimer(MatchStartTimerHandle, this, &APRGameState::TickMatchCountdown, 1.0f, true);
		UE_LOG(LogTemp, Warning, TEXT("60 Second ForceStartMatch"));
	}
}


void APRGameState::OnRep_MatchStart_CountDown()
{
	if (!HasAuthority()) return;
	UE_LOG(LogTemp, Warning, TEXT("CountDown: %d"), MatchStart_CountDown);
	OnMatchStart_CountDown.Broadcast(MatchStart_CountDown);
}

void APRGameState::OnRep_CurrentAmountOfPlayers()
{
	OnCurrentAmountOfPlayers.Broadcast(CurrentAmountOfPlayers);
}

void APRGameState::OnRep_MinimumRequirePlayers()
{
	OnMinimumRequirePlayers.Broadcast(MinimumRequirePlayers);
}

void APRGameState::EnableExtractionZones()
{
	if (!HasAuthority()) return;
	if (!ExtractionZoneClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ExtractionZoneClass is not set!"));
		return;
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(this, ExtractionZoneClass, FoundActors);

	int32 ActivatedCount = 0;

	for (AActor* Actor : FoundActors)
	{
		if (!Actor) continue;

		if (Actor->IsA(ExtractionZoneClass))
		{
			UFunction* ActivateFunc = Actor->FindFunction(FName("ActivateZone"));
			if (ActivateFunc)
			{
				Actor->ProcessEvent(ActivateFunc, nullptr);
				ActivatedCount++;
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Activated %d Extraction Zones"), ActivatedCount);


	// ; 
	SetMatchFlowState(EMatchFlowState::ExtractionEnabled); // 여기 추가

}

void APRGameState::TickMatchTimer()
{
	if (!HasAuthority()) return;
	if (RemainingMatchTime <= 0)return;
	RemainingMatchTime--;

	if (RemainingMatchTime <= 0)
	{
		MatchEnd();
		return;
	}
}

void APRGameState::TickExtractionTimer() // ; 탈출구 열리는 타이머 
{
	if (!HasAuthority()) return;
	if (RemainingExtractionTime <= 0) return;

	RemainingExtractionTime--;

	if (RemainingExtractionTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(ExtractionCountDownTimerHandle);
	}
}


void APRGameState::OnRep_RemainingMatchTime() const
{
	if (!HasAuthority())
	// UE_LOG(LogTemp, Warning, TEXT("남은 매치 시간: %d초"), RemainingMatchTime);
	OnRemainingMatchTime.Broadcast(RemainingMatchTime);
}

int32 APRGameState::GetRemainingExtractionTime() const
{
	return RemainingExtractionTime;
}

void APRGameState::OnRep_RemainingExtractionTime()  // 클라가 이함수 자동으로 호출 
{
	OnExtractionTimeUpdated.Broadcast(RemainingExtractionTime); // UI에서 바인딩할 수 있도록

}

void APRGameState::CallTheGmToEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(MatchEndTimerHandle);
	if (APRGameMode* GM = Cast<APRGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->EndThisMatch();
	}
}


//테스트!!!!!!!!!!!
void APRGameState::OnRep_MatchFlowState()
{
	// 플레이어에게 알려줌
	//for (APlayerState* PS : PlayerArray)
	//{
	//	if (APRPlayerController* PC = Cast<APRPlayerController>(PS->GetOwner()))
	//	{
	//		PC->HandleMatchFlowStateChanged(MatchFlowState);
	//	}
	//}


	//UE_LOG(LogTemp, Error, TEXT("Client OnRep_MatchFlowState called! State: %d"), static_cast<int32>(MatchFlowState));
	//UE_LOG(LogTemp, Error, TEXT("Client OnRep_MatchFlowState called! State: %d"), static_cast<int32>(MatchFlowState));

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (!PC->IsLocalController()) return;
		if (APRPlayerController* PRPC = Cast<APRPlayerController>(PC))
		{
			PRPC->HandleMatchFlowStateChanged(MatchFlowState);
		}
	}
}



void APRGameState::SetMatchFlowState(EMatchFlowState NewState)
{
	if (HasAuthority()/*서버인경우*/)
	{
		MatchFlowState = NewState;
		OnRep_MatchFlowState(); // 서버에서도 처리되도록 직접 호출
	}
}


