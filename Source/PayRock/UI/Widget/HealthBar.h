// PayRockGames

#pragma once

#include "CoreMinimal.h"
//#include "UI/Widget/BaseUserWidget.h"
#include "PayRock/UI/Widget/BaseUserWidget.h"
#include "PayRock/UI/WidgetController/OverlayWidgetController.h"

#include "HealthBar.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class PAYROCK_API UHealthBar : public UBaseUserWidget
{
	GENERATED_BODY()

private:
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthProgressBar;

    UPROPERTY()
    UOverlayWidgetController* OverlayWidgetController;

// 부모에 있다? 블루프린트에서 설정해줘양함
//public:
//    UFUNCTION()
//    void OnWidgetControllerSet(UOverlayWidgetController* Controller);

protected:
    virtual void NativeConstruct() override;

private:

    UFUNCTION()
    void OnHealthChanged(float NewHealth);

    UFUNCTION()
    void OnMaxHealthChanged(float NewMaxHealth);

    UFUNCTION()
    void UpdateHealthBar();



    float Health = 0.f;
    float MaxHealth = 1.f; // Avoid divide-by-zero

};
