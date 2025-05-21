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

   // ;!!! �Լ� �������ּ��� 
 
    // ��ġ ���� �� ���� �÷��̾� 
    // ��ġ ���� �� �䱸�Ǵ� �ּ� �÷��̾� �� 
    // ��ġ ���� ī��Ʈ �ٿ� 
    // 
    // 
    //GS->OnCurrentAmountOfPlayers.AddUObject(this, MatchHUD::UpdateCurrentPlayers);
    //GS->OnMinimumRequirePlayers.AddUObject(this, MatchHUD::UpdateMinimumPlayers);
    //GS->OnMatchStart_CountDown.AddUObject(this, MatchHUD::UpdateCountdown);
    
    // ? ���� ��Ī �ð�
    //GS->OnRemainingMatchTime.AddUObject(this, MatchHUD::UpdateRemainingTime);
    //
   
    // ������ ���� 2�ʸ��� ���� ���� (GameState�� Tick�� �ƴ�) ; 2�ʸ��� ȭ�鿡 �����ϱ� ���ؼ� 
   /* GetWorld()->GetTimerManager().SetTimer(AliveCheckTimerHandle, this, &UMatchHUD::UpdateAlivePlayers, 2.0f, true);*/

}

void UMatchHUD::OnWidgetControllerSet()
{
    Super::OnWidgetControllerSet();

    BindToGameState(); // ���⼭ ���ӽ�����Ʈ ���� ���ε� ����
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
