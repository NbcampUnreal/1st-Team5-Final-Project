// PayRockGames

#pragma once

#include "CoreMinimal.h"
//#include "UI/Widget/BaseUserWidget.h"
#include "BaseUserWidget.h"

//// ��� ��η� PRGameplayTags include
//#include "../../PRGameplayTags.h"
#include "PayRock/PRGameplayTags.h"

#include "InputSettingsWidget.generated.h"

class UButton;
class UInputMappingContext;
class UPRInputConfig;
struct FKey;
/**
  * Enhanced Input ��� Ű ���ε� UI ����
 */
UCLASS()
class PAYROCK_API UInputSettingsWidget : public UBaseUserWidget
{
	GENERATED_BODY()


protected:
    /** �����ε��� ��ư (����: ����) */
    UPROPERTY(meta = (BindWidget))
    UButton* JumpRebindButton;

    /** ���� �����ε� ���� �׼� �±� */
    FGameplayTag CurrentRebindingTag;

    /** �ܺο��� ���� (BP ���� ����) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (ExposeOnSpawn = true))
    TObjectPtr<UInputMappingContext> MappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (ExposeOnSpawn = true))
    TObjectPtr<UPRInputConfig> InputConfig;

    /** ��ư Ŭ�� �̺�Ʈ */
    UFUNCTION()
    void OnJumpRebindClicked();

    /** ���� �����ε� ó�� */
    void RebindAction(const FGameplayTag& InputTag, const FKey& NewKey);



public:
    virtual void NativeOnInitialized() override;
    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

};
