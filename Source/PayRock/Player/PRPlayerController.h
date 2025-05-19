// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PRPlayerController.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

DECLARE_MULTICAST_DELEGATE(FShowLeaveOptionsDelegate)

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
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> GuardAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditAnywhere, Category = "Input|Spector")
	TObjectPtr<UInputMappingContext> SpectorIMC;
	
	UPROPERTY(EditAnywhere, Category = "Input|Spectate")
	TObjectPtr<UInputAction> SpectateNextAction;

	UPROPERTY(EditAnywhere, Category = "Input|Spectate")
	TObjectPtr<UInputAction> SpectatePrevAction;
	
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> DeathOptionsWidgetClass;
	
	UFUNCTION(Client, Reliable)
	void Client_ShowDeathOptions();

	UFUNCTION(BlueprintCallable)
	void StartSpectating();
	void SpectateNext();
	void SpectatePrevious();

	FString GetNetModeAsString() const; // 디버깅용 함수
	FShowLeaveOptionsDelegate ShowLeaveOptionsDelegate;

	UFUNCTION(Client, Reliable)
	void Client_OnSpectateTargetDied(AActor* DeadActor);
	
	void OnSpectateTargetDied(AActor* DeadActor); // 관전 타겟 사망시 그 타겟 제거
protected:
	virtual void BeginPlay() override;

	void BindingSpector(); // 스펙터
private:
	void SetSpectateTarget(AActor* NewTarget); // 관전 타겟 설정

	
	UPROPERTY()
	TArray<AActor*> SpectateTargets;

	int32 CurrentSpectateIndex = -1;

	UPROPERTY()
	UUserWidget* DeathOptionsWidget;
};
