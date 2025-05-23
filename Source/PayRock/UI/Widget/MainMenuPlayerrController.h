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
	// ���� ǥ�� ���� ����
	UPROPERTY()
	UBaseUserWidget* CurrentHUDWidget;

	// �����Ϳ��� ������ ������
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UBaseUserWidget> MainMenuHUDClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UBaseUserWidget> MatchHUDClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UBaseUserWidget> InGameHUDClass;

protected:
	virtual void BeginPlay() override;


public:
	// �ܺο��� ȣ��Ǵ� ��ȯ �Լ�
	UFUNCTION(BlueprintCallable)
	void StartGame(); // Start ��ư ������ ȣ��

	UFUNCTION(BlueprintCallable)
	void OnMatchStarted(); // ��ġ ���۵Ǹ� ȣ��

	// ���� ��ȯ �Լ�
	void ShowMainMenuHUD();
	void ShowMatchHUD();
	void ShowInGameHUD();
	void ClearCurrentHUD();
};
