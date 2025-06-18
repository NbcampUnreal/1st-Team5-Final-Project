// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PayRock/GameSystem/PRGameState.h"
#include "PRPlayerController.generated.h"


struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class UUserWidget;
//class EMatchFlowState; // Enum class는 전방선언 불가;; 

UCLASS()
class PAYROCK_API APRPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APRPlayerController();

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> PlayerIMC;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> CrouchAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> GuardAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> AimAction;

	// ESC 바인딩 
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ToggleMenuAction;

	// 관전
	UPROPERTY(EditAnywhere, Category = "Input|Spector")
	TObjectPtr<UInputMappingContext> SpectorIMC;
	
	UPROPERTY(EditAnywhere, Category = "Input|Spectate")
	TObjectPtr<UInputAction> SpectateNextAction;

	UPROPERTY(EditAnywhere, Category = "Input|Spectate")
	TObjectPtr<UInputAction> SpectatePrevAction;

	// 사망/탈출 UI
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> DeathOptionsWidgetClass;
	
	UFUNCTION(Client, Reliable)
	void Client_ShowDeathOptions();
	void RemoveDeathOptions();

	UFUNCTION(BlueprintCallable)
	void StartSpectating();
	void SpectateNext();
	void SpectatePrevious();

	FString GetNetModeAsString() const; // 디버깅용 함수

	UFUNCTION(Client, Reliable)
	void Client_OnSpectateTargetDied(AActor* DeadActor);
	
	void OnSpectateTargetDied(AActor* DeadActor); // 관전 타겟 사망시 그 타겟 제거

protected:
	virtual void BeginPlay() override;

	// esc 바인딩 위한 함수 
	virtual void SetupInputComponent() override;
	void BindingSpector(); // 스펙터

private:
	void SetSpectateTarget(AActor* NewTarget); // 관전 타겟 설정
	
	UPROPERTY()
	TArray<AActor*> SpectateTargets;

	int32 CurrentSpectateIndex = -1;

	UPROPERTY()
	UUserWidget* DeathOptionsWidget;
	
//=========================================================================
	// 컨트롤러 내부에 추가:
private:

	// 컨트롤러에서-> 세팅메뉴 클래스 설정가능 
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SettingsMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> SettingsMenuWidget;

	// ;ESC 장비창 끄기 
	UPROPERTY()
	TWeakObjectPtr<UUserWidget> InventoryWidget;

	// ;헬프키 사용
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShowHelpAction;  // H 키용

	bool bIsSettingsMenuOpen = false;

	void ToggleHelpMenu();

public:
	// ; 메인메뉴 옵션 에서도 사용하기 위해서 public 
	void ToggleSettingsMenu(); // ESC 키 입력 시 호출할 함수

public:
	// 매칭, 매치후 인게임 UI 다르게 표시 
	void HandleMatchFlowStateChanged(EMatchFlowState NewState);

	/*
	 *	Necro Character
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerRequestNecroCharacter();
	UFUNCTION(Client, Reliable)
	void ClientOnNecroPossessed();
};
