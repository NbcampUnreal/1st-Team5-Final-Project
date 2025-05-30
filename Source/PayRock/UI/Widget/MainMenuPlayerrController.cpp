// PayRockGames


#include "PayRock/UI/Widget/MainMenuPlayerrController.h"
#include "PayRock/UI/Widget/BaseUserWidget.h"

void AMainMenuPlayerrController::BeginPlay()
{
	// 1. 메인 메뉴 레벨 시작
	// 레벨 기반 HUD 전환
	FName LevelName = FPackageName::GetShortFName(GetWorld()->GetMapName());

	if (LevelName == "MainMenuLevel")
	{
		// 메뉴레벨에 진입후 메인메뉴 HUD 띄우기
		ShowMainMenuHUD();
	}
	/*else if (LevelName == "MatchWaitingLevel")
	{
		ShowMatchHUD();
	}*/
}






//=============================
// 메인메뉴레벨(HUD) -> 로비(HUD) -> 로딩 -> 매칭레벨(HUD) -> 인게임레벨(인게임UI)
void AMainMenuPlayerrController::ShowMainMenuHUD()
{
	// 있던 UI 제거 
	ClearCurrentHUD();


	if (MainMenuHUDClass)
	{
		// 위젯블프에서 설정한 메인메뉴 HUD 실행 
		CurrentHUDWidget = CreateWidget<UBaseUserWidget>(this, MainMenuHUDClass);
		if (CurrentHUDWidget)
		{
			CurrentHUDWidget->AddToViewport();
		}
	}
}


void AMainMenuPlayerrController::StartGame()
{
	// 메뉴레벨에서 
	// 매치 대기 레벨로 이동 (Level 이름은 에디터에서 사용하는 맵 이름과 일치해야 함)
	//UGameplayStatics::OpenLevel(this, FName("MatchWaitingLevel"));

	// ; 스타트게임은 메인메뉴UI에서 레벨 변경
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
