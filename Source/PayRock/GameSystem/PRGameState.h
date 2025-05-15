// PayRockGames PRGameState.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PRIGameState.h"
#include "PRGameState.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API APRGameState : public AGameState, public IPRIGameState
{
	GENERATED_BODY()
public:

protected:
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


public:
	APRGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void Notify_PlayerConnection_Implementation() override;
protected:

	void TickMatchCountdown(); // 카운트 다운

	void StartMatch(); // 카운트 다운 후 매치 스타트

	void ForceStartMatch(); // 조건이 불충족되어도 강제 매치 스타트
	
	UFUNCTION()
	void OnRep_MatchStart_CountDown();
};
