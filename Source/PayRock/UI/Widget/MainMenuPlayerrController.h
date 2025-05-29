// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuPlayerrController.generated.h"
class UBaseUserWidget;
/**
 * 
 */
UCLASS()
class PAYROCK_API AMainMenuPlayerrController : public APlayerController
{
	GENERATED_BODY()
	

private:
	// 현재 표시 중인 위젯
	UPROPERTY()
	UBaseUserWidget* CurrentHUDWidget;

	// 에디터에서 설정할 위젯들
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UBaseUserWidget> MainMenuHUDClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UBaseUserWidget> MatchHUDClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UBaseUserWidget> InGameHUDClass;

protected:
	virtual void BeginPlay() override;


public:
	// 외부에서 호출되는 전환 함수
	UFUNCTION(BlueprintCallable)
	void StartGame(); // Start 버튼 누르면 호출

	UFUNCTION(BlueprintCallable)
	void OnMatchStarted(); // 매치 시작되면 호출

	// 위젯 전환 함수
	void ShowMainMenuHUD();
	void ShowMatchHUD();
	void ShowInGameHUD();
	void ClearCurrentHUD();
};
