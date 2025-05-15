// PayRockGames PRGameState.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PRIGameState.h"
#include "PRGameState.generated.h"

UENUM(BlueprintType)
enum class EMatchFlowState : uint8
{
	WaitingToStart,
	MatchInProgress,
	ExtractionEnabled,
	MatchEnded
};


UCLASS()
class PAYROCK_API APRGameState : public AGameState, public IPRIGameState
{
	GENERATED_BODY()
public:

protected:
	///////////////매치 시작 전 ////////////////////////////////////////////
	UPROPERTY(BlueprintReadWrite, Replicated, EditAnywhere, Category = "MatchFlow|Players")
	int32 MinimumRequirePlayers;

	UPROPERTY(BlueprintReadWrite, Replicated, EditAnywhere, Category = "MatchFlow|Players")
	int32 CurrentAmountOfPlayers;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_MatchStart_CountDown, EditAnywhere, Category = "MatchFlow|Players")
	int32 MatchStart_CountDown;

	bool bHasStartedCountdown = false;
	bool bForceStarted = false;
	
	FTimerHandle MatchStartTimerHandle;
	FTimerHandle ForceStartTimerHandle;
	/////////////////////////////////////////////////////////////////////
	
	
	///////////////매치 시작 후 ////////////////////////////////////////////
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MatchFlow|Timing")
	int32 MatchDurationSeconds;				// 매치 진행 시간 (디버깅때는 2분)

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MatchFlow|Timing")
	int32 ExtractionActivationTime ;

	UPROPERTY(ReplicatedUsing = OnRep_RemainingMatchTime, BlueprintReadOnly, Category = "MatchFlow|Timing")
	int32 RemainingMatchTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MatchFlow|Extraction")
	TSubclassOf<AActor> ExtractionZoneClass;
	
	FTimerHandle MatchTimerHandle;

	FTimerHandle AliveCheckTimerHandle;

	FTimerHandle ExtractionActivationTimerHandle;
	/////////////////////////////////////////////////////////////////////

public:
	APRGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	///////////////매치 시작 전 ////////////////////////////////////////////
	
	virtual void Notify_PlayerConnection_Implementation() override;
	
	//////////////////////////////////////////////////////////////////////
	
	
	///////////////매치 시작 후 ////////////////////////////////////////////
	int32 GetAlivePlayerCount() const; // 살아있는 플레이어 카운트

	void CheckAlivePlayers(); // 플레이어가 모두 사망했는지 체크
	
	void MatchEnd() const; // 매치 끝
	//////////////////////////////////////////////////////////////////////
	
protected:

	///////////////매치 시작 전 ////////////////////////////////////////////
	void TickMatchCountdown(); // 카운트 다운

	void StartMatch(); // 카운트 다운 후 매치 스타트

	void ForceStartMatch(); // 조건이 불충족되어도 강제 매치 스타트
	
	UFUNCTION()
	void OnRep_MatchStart_CountDown();
	//////////////////////////////////////////////////////////////////////

	///////////////매치 시작 후 ////////////////////////////////////////////
	void EnableExtractionZones(); // 탈출구 Open

	void TickMatchTimer();
	UFUNCTION()
	void OnRep_RemainingMatchTime();
	/////////////////////////////////////////////////////////////////////
};
