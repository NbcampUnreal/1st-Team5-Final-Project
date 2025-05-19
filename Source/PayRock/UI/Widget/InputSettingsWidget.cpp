// PayRockGames


#include "PayRock/UI/Widget/InputSettingsWidget.h"
#include "Components/Button.h"
#include "InputMappingContext.h"
#include "PayRock/Input/PRInputConfig.h"
#include "PayRock/PRGameplayTags.h"  // 태그 싱글톤
#include "EnhancedInputSubsystems.h"

void UInputSettingsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (JumpRebindButton)
    {
        JumpRebindButton->OnClicked.AddDynamic(this, &UInputSettingsWidget::OnJumpRebindClicked);
    }
}

void UInputSettingsWidget::OnJumpRebindClicked()
{
    // 태그 설정: 지금은 Jump 액션을 InputTag_1으로 가정
    CurrentRebindingTag = FPRGameplayTags::Get().InputTag_1;

    // 키보드 입력을 받을 준비
    SetKeyboardFocus();
}

FReply UInputSettingsWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    if (CurrentRebindingTag.IsValid())
    {
        const FKey PressedKey = InKeyEvent.GetKey();

        // 리바인딩 실행
        RebindAction(CurrentRebindingTag, PressedKey);

        // 상태 초기화
        CurrentRebindingTag = FGameplayTag();
        return FReply::Handled();
    }

    return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UInputSettingsWidget::RebindAction(const FGameplayTag& InputTag, const FKey& NewKey)
{
//    if (!MappingContext || !InputConfig)
//    {
//        UE_LOG(LogTemp, Warning, TEXT("RebindAction failed: MappingContext or InputConfig is null."));
//        return;
//    }
//
//    const FPRInputAction* Action = InputConfig->FindAbilityActionByTag(InputTag);
//    if (!Action || !Action->InputAction)
//    {
//        UE_LOG(LogTemp, Warning, TEXT("No InputAction found for tag: %s"), *InputTag.ToString());
//        return;
//    }
//
//#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
//    MappingContext->UnmapAllKeysForAction(Action->InputAction); // 5.3 이상에서 제공
//#endif
//
//    MappingContext->MapKey(Action->InputAction, NewKey);
//
//    UE_LOG(LogTemp, Log, TEXT("Rebound action [%s] to key [%s]"), *InputTag.ToString(), *NewKey.ToString());
}