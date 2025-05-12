// PayRockGames

#pragma once

#include "CoreMinimal.h"
//#include "UI/Widget/BaseUserWidget.h"
#include "BaseUserWidget.h"

//// 상대 경로로 PRGameplayTags include
//#include "../../PRGameplayTags.h"
#include "PayRock/PRGameplayTags.h"

#include "InputSettingsWidget.generated.h"

class UButton;
class UInputMappingContext;
class UPRInputConfig;
struct FKey;
/**
  * Enhanced Input 기반 키 바인딩 UI 위젯
 */
UCLASS()
class PAYROCK_API UInputSettingsWidget : public UBaseUserWidget
{
	GENERATED_BODY()


protected:
    /** 리바인딩할 버튼 (예시: 점프) */
    UPROPERTY(meta = (BindWidget))
    UButton* JumpRebindButton;

    /** 현재 리바인딩 중인 액션 태그 */
    FGameplayTag CurrentRebindingTag;

    /** 외부에서 주입 (BP 노출 가능) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (ExposeOnSpawn = true))
    TObjectPtr<UInputMappingContext> MappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (ExposeOnSpawn = true))
    TObjectPtr<UPRInputConfig> InputConfig;

    /** 버튼 클릭 이벤트 */
    UFUNCTION()
    void OnJumpRebindClicked();

    /** 실제 리바인딩 처리 */
    void RebindAction(const FGameplayTag& InputTag, const FKey& NewKey);



public:
    virtual void NativeOnInitialized() override;
    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

};
