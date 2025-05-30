// PayRockGames

#pragma once

#include "CoreMinimal.h"
//#include "UI/Widget/BaseUserWidget.h"
#include "BaseUserWidget.h"
#include "VideoSettingsWidget.generated.h"

class UComboBoxString;
class USlider;
class UButton;
class UMainMenuUserWidget;
class UOptionsMenuWidget;
/**
 * ���� ���� �� UI ����
 */
UCLASS()
class PAYROCK_API UVideoSettingsWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
protected:
    /** �ػ� ���� �޺��ڽ� */
    UPROPERTY(meta = (BindWidget))
    UComboBoxString* ResolutionComboBox;

    /** ��üȭ�� ��� ���� �޺��ڽ� */
    UPROPERTY(meta = (BindWidget))
    UComboBoxString* FullscreenComboBox;

    /** �׷��� ǰ�� �����̴� */
    UPROPERTY(meta = (BindWidget))
    USlider* QualitySlider;

    /** ���� ��ư */
    UPROPERTY(meta = (BindWidget))
    UButton* ApplyButton;

    /* �ڷ� ���� ��ư -> �ΰ��ӿ��� ���� �ϱ⿡ �ɼų�*/
    UPROPERTY(meta = (BindWidgetOptional))
    UButton* BackButton;

public:
    // MainMenu ������ ���� ���� (SetVisibility�� ���� �ʿ�)
    UPROPERTY()
    UMainMenuUserWidget* MainMenuRef;

    UPROPERTY()
    UOptionsMenuWidget* OptionsMenuRef;

protected:
    /** ��ư Ŭ�� ó�� */
    UFUNCTION()
    void OnApplyClicked();

    /** �ڷ� ���� ��ư **/
    UFUNCTION()
    void OnBackClicked();

    /** ���� ��ƿ: ���� �� ���� */
    void ApplySettings();

    /** ���� ��ƿ: �޺��ڽ� �� �Ľ� �� ���� */
    FIntPoint ParseResolution(const FString& ResolutionString) const;


public:
    virtual void NativeOnInitialized() override;
};


/** ================= 1. UPROPERTY (���÷��� �ý��� ����) ================= **/

/** ================= 2. UFUNCTION (���÷��� �ý��� �Լ�) ================= **/

/** ================= 3. ������ �� �ʼ� �������̵� �Լ� ================= **/

/** ================= 4. �Ϲ� �޼��� (���÷����� �ʿ� ���� �Լ�) ================= **/

/** ================= 5. �Ϲ� ��� ���� (���÷����� �ʿ� ���� ����) ================= **/
