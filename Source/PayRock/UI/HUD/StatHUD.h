// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "StatHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UStatWidgetController;
class UBaseUserWidget;

UCLASS()
class PAYROCK_API AStatHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UBaseUserWidget> StatWidget;

	UFUNCTION(BlueprintCallable)
	UStatWidgetController* GetStatWidgetController(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable)
	void InitStatWidget(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> StatWidgetClass;
	UPROPERTY()
	TObjectPtr<UStatWidgetController> StatWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UStatWidgetController> StatWidgetControllerClass;
};
