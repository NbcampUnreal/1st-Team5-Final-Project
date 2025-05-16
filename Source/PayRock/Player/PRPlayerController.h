// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PRPlayerController.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

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

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> DeathOptionsWidgetClass;
	
	UFUNCTION(Client, Reliable)
	void Client_ShowDeathOptions();

	UFUNCTION(BlueprintCallable)
	void StartSpectating();
	void SpectateNext();
	void SpectatePrevious();
	
protected:
	virtual void BeginPlay() override;

private:
	void SetSpectateTarget(AActor* NewTarget);

	UPROPERTY()
	TArray<AActor*> SpectateTargets;

	int32 CurrentSpectateIndex = -1;

	UPROPERTY()
	UUserWidget* DeathOptionsWidget;
};
