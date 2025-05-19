// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UBaseUserWidget;
class UOverlayWidgetController;
struct FWidgetControllerParams;

UCLASS()
class PAYROCK_API ABaseHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UBaseUserWidget> InGameHUDWidget;

	UFUNCTION(BlueprintCallable)
	UOverlayWidgetController* GetInGameHUDWidgetController(const FWidgetControllerParams& WCParams);

	void InitInGameHUD(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> InGameHUDWidgetClass;
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> InGameHUDWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> InGameHUDWidgetControllerClass;
};
