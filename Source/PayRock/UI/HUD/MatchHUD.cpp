// PayRockGames


#include "MatchHUD.h"
#include "PayRock/GameSystem/PRGameState.h"
#include "Components/TextBlock.h"

void UMatchHUD::NativeConstruct()
{
	Super::NativeConstruct();

	BindToGameState();
}

void UMatchHUD::BindToGameState()
{
    APRGameState* GS = GetWorld()->GetGameState<APRGameState>();
    if (!GS) return;

   // ;!!! 함수 구현해주세요 
 
    // 매치 시작 전 현재 플레이어 
    // 매치 시작 전 요구되는 최소 플레이어 수 
    // 매치 시작 카운트 다운 
    // 
    // 
    //GS->OnCurrentAmountOfPlayers.AddUObject(this, MatchHUD::UpdateCurrentPlayers);
    //GS->OnMinimumRequirePlayers.AddUObject(this, MatchHUD::UpdateMinimumPlayers);
    //GS->OnMatchStart_CountDown.AddUObject(this, MatchHUD::UpdateCountdown);
    
    // ? 남은 매칭 시간
    //GS->OnRemainingMatchTime.AddUObject(this, MatchHUD::UpdateRemainingTime);
    //
   
    // 생존자 수는 2초마다 직접 갱신 (GameState의 Tick이 아님) ; 2초마다 화면에 갱신하기 위해서 
   /* GetWorld()->GetTimerManager().SetTimer(AliveCheckTimerHandle, this, &UMatchHUD::UpdateAlivePlayers, 2.0f, true);*/

}

void UMatchHUD::OnWidgetControllerSet()
{
    Super::OnWidgetControllerSet();

    BindToGameState(); // 여기서 게임스테이트 정보 바인딩 실행
}

void UMatchHUD::UpdateCurrentPlayers(int32 NewCount)
{
    if (Text_CurrentPlayers)
        Text_CurrentPlayers->SetText(FText::AsNumber(NewCount));
}

void UMatchHUD::UpdateMinimumPlayers(int32 NewMin)
{
    if (Text_MinimumPlayers)
        Text_MinimumPlayers->SetText(FText::AsNumber(NewMin));
}

void UMatchHUD::UpdateCountdown(int32 Countdown)
{
    if (Text_Countdown)
        Text_Countdown->SetText(FText::AsNumber(Countdown));
}

void UMatchHUD::UpdateRemainingTime(int32 TimeLeft)
{
    //if (Text_RemainingTime)
    //    Text_RemainingTime->SetText(FText::AsNumber(TimeLeft));
}

void UMatchHUD::UpdateAlivePlayers()
{
    //APRGameState* GS = GetWorld()->GetGameState<APRGameState>();
    //if (!GS) return;

    //int32 AliveCount = GS->GetAlivePlayerCount();
    //if (Text_AlivePlayers)
    //    Text_AlivePlayers->SetText(FText::AsNumber(AliveCount));
}
