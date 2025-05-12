// PayRockGames


#include "PayRock/UI/Widget/InputSettingsWidget.h"
#include "Components/Button.h"
#include "InputMappingContext.h"
#include "PayRock/Input/PRInputConfig.h"
#include "PayRock/PRGameplayTags.h"  // �±� �̱���
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
    // �±� ����: ������ Jump �׼��� InputTag_1���� ����
    CurrentRebindingTag = FPRGameplayTags::Get().InputTag_1;

    // Ű���� �Է��� ���� �غ�
    SetKeyboardFocus();
}

FReply UInputSettingsWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    if (CurrentRebindingTag.IsValid())
    {
        const FKey PressedKey = InKeyEvent.GetKey();

        // �����ε� ����
        RebindAction(CurrentRebindingTag, PressedKey);

        // ���� �ʱ�ȭ
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
//    MappingContext->UnmapAllKeysForAction(Action->InputAction); // 5.3 �̻󿡼� ����
//#endif
//
//    MappingContext->MapKey(Action->InputAction, NewKey);
//
//    UE_LOG(LogTemp, Log, TEXT("Rebound action [%s] to key [%s]"), *InputTag.ToString(), *NewKey.ToString());
}