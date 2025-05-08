// PayRockGames

#pragma once

#include "CoreMinimal.h"
//#include "UI/Widget/BaseUserWidget.h"
#include "BaseUserWidget.h"
#include "OptionsMenuWidget.generated.h"

/**
 *  �ɼ� �޴� ���� ���� (�� ��ȯ ����)
 */

class UButton;
class UWidgetSwitcher;
UCLASS()
class PAYROCK_API UOptionsMenuWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
protected:
    // �� ��ȯ�� ��ư��
    UPROPERTY(meta = (BindWidget))
    UButton* VideoButton;

    UPROPERTY(meta = (BindWidget))
    UButton* AudioButton;

    UPROPERTY(meta = (BindWidget))
    UButton* InputButton;

    UPROPERTY(meta = (BindWidget))
    UButton* GameplayButton;

    // �� ������ ��ȯ�� ���� ����ó
    UPROPERTY(meta = (BindWidget))
    UWidgetSwitcher* TabSwitcher;

    // Ŭ�� �ڵ鷯��
    UFUNCTION()
    void OnVideoButtonClicked();

    UFUNCTION()
    void OnAudioButtonClicked();

    UFUNCTION()
    void OnInputButtonClicked();

    UFUNCTION()
    void OnGameplayButtonClicked();

    // �ε����� ���� �ɼ� �� �ٲ� 
    void SwitchTab(int32 TabIndex);


public:
	virtual void NativeOnInitialized() override;

};


/** ================= 1. UPROPERTY (���÷��� �ý��� ����) ================= **/

/** ================= 2. UFUNCTION (���÷��� �ý��� �Լ�) ================= **/

/** ================= 3. ������ �� �ʼ� �������̵� �Լ� ================= **/

/** ================= 4. �Ϲ� �޼��� (���÷����� �ʿ� ���� �Լ�) ================= **/

/** ================= 5. �Ϲ� ��� ���� (���÷����� �ʿ� ���� ����) ================= **/
