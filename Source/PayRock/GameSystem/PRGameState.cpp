// PayRockGames PRGameState.cpp


#include "PRGameState.h"
#include "PRGameMode.h"
#include "UObject/UObjectGlobals.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameModeBase.h"


APRGameState::APRGameState()
{
	MinimumRequirePlayers = 2;
	CurrentAmountOfPlayers = 0;
	MatchStart_CountDown = -1;
	bReplicates = true;
}

void APRGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APRGameState, CurrentAmountOfPlayers);
	DOREPLIFETIME(APRGameState, MinimumRequirePlayers);
	DOREPLIFETIME(APRGameState, MatchStart_CountDown);
}

void APRGameState::Notify_PlayerConnection_Implementation()
{
	if (!HasAuthority()) return;

	// 최신 접속 인원 수
	CurrentAmountOfPlayers = GetWorld()->GetGameState<AGameState>()->PlayerArray.Num();

	// 40초 강제 시작 타이머가 없다면 생성
	if (!bForceStarted && !GetWorld()->GetTimerManager().IsTimerActive(ForceStartTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(ForceStartTimerHandle, this, &APRGameState::ForceStartMatch, 40.0f, false);
	}

	// 조건 충족 시
	if (CurrentAmountOfPlayers >= MinimumRequirePlayers)
	{
		if (!bHasStartedCountdown)
		{
			// 카운트다운 시작
			bHasStartedCountdown = true;
			MatchStart_CountDown = 5;
			GetWorld()->GetTimerManager().SetTimer(MatchStartTimerHandle, this, &APRGameState::TickMatchCountdown, 1.0f, true);
		}
		else
		{
			// 이미 카운트다운 중이라면: 남은 시간이 15초보다 크면 15초로 "줄임"
			if (MatchStart_CountDown > 5)
			{
				MatchStart_CountDown = 5;
			}
		}
	}
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
		if (!Pawn)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Pawn found for %s"), *PS->GetPlayerName());
			continue;
		}

		AActor* StartSpot = GM->ChooseMatchStartSpot();
		if (!StartSpot)
		{
			UE_LOG(LogTemp, Warning, TEXT("No StartSpot available."));
			continue;
		}

		// 로그로 위치 출력
		UE_LOG(LogTemp, Warning, TEXT("Teleporting %s to %s"), *PS->GetPlayerName(), *StartSpot->GetActorLocation().ToString());

		// 순간이동
		Pawn->SetActorLocationAndRotation(
			StartSpot->GetActorLocation(),
			StartSpot->GetActorRotation(),
			false, // sweep
			nullptr,
			ETeleportType::TeleportPhysics
		);
	}
}

void APRGameState::ForceStartMatch()
{
	if (!bHasStartedCountdown)
	{
		bHasStartedCountdown = true;
		bForceStarted = true;
		MatchStart_CountDown = 5;
		GetWorld()->GetTimerManager().SetTimer(MatchStartTimerHandle, this, &APRGameState::TickMatchCountdown, 1.0f, true);
		UE_LOG(LogTemp, Warning, TEXT("40 Second ForceStartMatch"));
	}
}

void APRGameState::OnRep_MatchStart_CountDown()
{
	UE_LOG(LogTemp, Warning, TEXT("CountDown: %d"), MatchStart_CountDown);
}

