// PayRockGames


//#include "UI/Widget/HealthBar.h"

#include "PayRock/UI/Widget/BaseUserWidget.h"
#include "Components/ProgressBar.h"
#include "HealthBar.h"


void UHealthBar::NativeConstruct()
{
    Super::NativeConstruct();

    // 초기값 설정 (옵션)
    UpdateHealthBar();
}

void UHealthBar::OnHealthChanged(float NewHealth)
{
    Health = NewHealth;
    UpdateHealthBar();
}

void UHealthBar::OnMaxHealthChanged(float NewMaxHealth)
{
    MaxHealth = NewMaxHealth;
    UpdateHealthBar();
}

void UHealthBar::UpdateHealthBar()
{
    if (!HealthProgressBar) return;

    float Percent = (MaxHealth > 0.f) ? Health / MaxHealth : 0.f;
    HealthProgressBar->SetPercent(Percent);
}


// 
//void UHealthBar::OnWidgetControllerSet()
//{
//    OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController);
//    if (!OverlayWidgetController) return;
//
//    OverlayWidgetController->OnHealthChanged.AddDynamic(this, &UHealthBar::OnHealthChanged);
//    OverlayWidgetController->OnMaxHealthChanged.AddDynamic(this, &UHealthBar::OnMaxHealthChanged);
//}

//void UHealthBar::OnWidgetControllerSet(UOverlayWidgetController* Controller)
//{
//    if (!Controller) return;
//
//    OverlayWidgetController = Controller;
//
//    // 바인딩
//   /* OverlayWidgetController->OnHealthChanged.AddUObject(this, &UMyWidget::OnHealthChanged);
//    OverlayWidgetController->OnMaxHealthChanged.AddUObject(this, &UMyWidget::OnMaxHealthChanged);*/
//    OverlayWidgetController->OnHealthChanged.AddDynamic(this, &UHealthBar::OnHealthChanged);
//    OverlayWidgetController->OnMaxHealthChanged.AddDynamic(this, &UHealthBar::OnMaxHealthChanged);
//}
