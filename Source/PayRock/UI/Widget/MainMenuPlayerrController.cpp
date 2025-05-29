// PayRockGames


#include "PayRock/UI/Widget/MainMenuPlayerrController.h"
#include "PayRock/UI/Widget/BaseUserWidget.h"

void AMainMenuPlayerrController::BeginPlay()
{
	// 1. ���� �޴� ���� ����
	// ���� ��� HUD ��ȯ
	FName LevelName = FPackageName::GetShortFName(GetWorld()->GetMapName());

	if (LevelName == "MainMenuLevel")
	{
		// �޴������� ������ ���θ޴� HUD ����
		ShowMainMenuHUD();
	}
	/*else if (LevelName == "MatchWaitingLevel")
	{
		ShowMatchHUD();
	}*/
}






//=============================
// ���θ޴�����(HUD) -> �κ�(HUD) -> �ε� -> ��Ī����(HUD) -> �ΰ��ӷ���(�ΰ���UI)
void AMainMenuPlayerrController::ShowMainMenuHUD()
{
	// �ִ� UI ���� 
	ClearCurrentHUD();


	if (MainMenuHUDClass)
	{
		// ������������ ������ ���θ޴� HUD ���� 
		CurrentHUDWidget = CreateWidget<UBaseUserWidget>(this, MainMenuHUDClass);
		if (CurrentHUDWidget)
		{
			CurrentHUDWidget->AddToViewport();
		}
	}
}


void AMainMenuPlayerrController::StartGame()
{
	// �޴��������� 
	// ��ġ ��� ������ �̵� (Level �̸��� �����Ϳ��� ����ϴ� �� �̸��� ��ġ�ؾ� ��)
	//UGameplayStatics::OpenLevel(this, FName("MatchWaitingLevel"));

	// ; ��ŸƮ������ ���θ޴�UI���� ���� ����
}

void AMainMenuPlayerrController::OnMatchStarted()
{
}



void AMainMenuPlayerrController::ShowMatchHUD()
{
}

void AMainMenuPlayerrController::ShowInGameHUD()
{
}

void AMainMenuPlayerrController::ClearCurrentHUD()
{
	if (CurrentHUDWidget && CurrentHUDWidget->IsInViewport())
	{
		CurrentHUDWidget->RemoveFromParent();
		CurrentHUDWidget = nullptr;
	}
}
