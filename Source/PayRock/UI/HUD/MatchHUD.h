// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/UI/Widget/BaseUserWidget.h"
#include "MatchHUD.generated.h"

/**
 * 
 */

class UTextBlock;

UCLASS()
class PAYROCK_API UMatchHUD : public UBaseUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;

	void BindToGameState(); // ���ӽ�����Ʈ���� ���� ���ε�

protected:
	virtual void OnWidgetControllerSet() override;

private:
	// �ؽ�Ʈ ǥ�ÿ� ������
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_CurrentPlayers;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_MinimumPlayers;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Countdown;

	//UPROPERTY(meta = (BindWidget))
	//UTextBlock* Text_RemainingTime;

	//UPROPERTY(meta = (BindWidget))
	//UTextBlock* Text_AlivePlayers;

	FTimerHandle AliveCheckTimerHandle;

	void UpdateCurrentPlayers(int32 NewCount);
	void UpdateMinimumPlayers(int32 NewMin);
	void UpdateCountdown(int32 Countdown);
	void UpdateRemainingTime(int32 TimeLeft);
	void UpdateAlivePlayers();


};
