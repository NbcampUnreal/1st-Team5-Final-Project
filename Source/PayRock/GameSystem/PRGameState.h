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

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStart_CountDown, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentAmountOfPlayers, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMinimumRequirePlayers, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRemainingMatchTime, int32);


UCLASS()
class PAYROCK_API APRGameState : public AGameState, public IPRIGameState
{
	GENERATED_BODY()
public:

	int32 GetMinimumRequirePlayers();

	int32 GetCurrentAmountOfPlayers();

	int32 GetMatchStartCountDown();

	int32 GetMatchDurationSeconds();

	int32 GetExtractionActivationTime();

	int32 GetRemainingTime();
	
protected:
	///////////////매치 시작 전 ////////////////////////////////////////////
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_MinimumRequirePlayers, EditAnywhere, Category = "MatchFlow|Players")
	int32 MinimumRequirePlayers;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_CurrentAmountOfPlayers, EditAnywhere, Category = "MatchFlow|Players")
	int32 CurrentAmountOfPlayers;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_MatchStart_CountDown, EditAnywhere, Category = "MatchFlow|Players")
	int32 MatchStart_CountDown;

	bool bHasStartedCountdown = false;
	bool bForceStarted = false;

	FOnMatchStart_CountDown OnMatchStart_CountDown;
	FOnCurrentAmountOfPlayers OnCurrentAmountOfPlayers;
	FOnMinimumRequirePlayers OnMinimumRequirePlayers;
	
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


	FOnRemainingMatchTime OnRemainingMatchTime;
	
	FTimerHandle MatchTimerHandle;

	FTimerHandle AliveCheckTimerHandle;

	FTimerHandle ExtractionActivationTimerHandle;

	FTimerHandle MatchEndTimerHandle;
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
	
	void MatchEnd(); // 매치 끝
	//////////////////////////////////////////////////////////////////////
	
protected:

	///////////////매치 시작 전 ////////////////////////////////////////////
	void TickMatchCountdown(); // 카운트 다운

	void StartMatch(); // 카운트 다운 후 매치 스타트

	void ForceStartMatch(); // 조건이 불충족되어도 강제 매치 스타트
	
	UFUNCTION()
	void OnRep_MatchStart_CountDown();

	UFUNCTION()
	void OnRep_CurrentAmountOfPlayers();

	UFUNCTION()
	void OnRep_MinimumRequirePlayers();
	//////////////////////////////////////////////////////////////////////

	///////////////매치 시작 후 ////////////////////////////////////////////
	void EnableExtractionZones(); // 탈출구 Open

	void TickMatchTimer();
	
	UFUNCTION()
	void OnRep_RemainingMatchTime() const;

	void CallTheGmToEnd();
	/////////////////////////////////////////////////////////////////////
	//테스트!!!!!!!!!!!!!!!
	UPROPERTY(ReplicatedUsing = OnRep_MatchFlowState)
	EMatchFlowState MatchFlowState;

	UFUNCTION()
	void OnRep_MatchFlowState();


	void SetMatchFlowState(EMatchFlowState NewState);



};
