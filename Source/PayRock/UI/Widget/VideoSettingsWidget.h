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
 * 비디오 설정 탭 UI 위젯
 */
UCLASS()
class PAYROCK_API UVideoSettingsWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
protected:
    /** 해상도 선택 콤보박스 */
    UPROPERTY(meta = (BindWidget))
    UComboBoxString* ResolutionComboBox;

    /** 전체화면 모드 선택 콤보박스 */
    UPROPERTY(meta = (BindWidget))
    UComboBoxString* FullscreenComboBox;

    /** 그래픽 품질 슬라이더 */
    UPROPERTY(meta = (BindWidget))
    USlider* QualitySlider;

    /** 적용 버튼 */
    UPROPERTY(meta = (BindWidget))
    UButton* ApplyButton;

    /* 뒤로 가기 버튼 -> 인게임에서 재사용 하기에 옵셔널*/
    UPROPERTY(meta = (BindWidgetOptional))
    UButton* BackButton;

public:
    // MainMenu 위젯에 대한 참조 (SetVisibility를 위해 필요)
    UPROPERTY()
    UMainMenuUserWidget* MainMenuRef;

    UPROPERTY()
    UOptionsMenuWidget* OptionsMenuRef;

protected:
    /** 버튼 클릭 처리 */
    UFUNCTION()
    void OnApplyClicked();

    /** 뒤로 가기 버튼 **/
    UFUNCTION()
    void OnBackClicked();

    /** 내부 유틸: 설정 값 적용 */
    void ApplySettings();

    /** 내부 유틸: 콤보박스 값 파싱 → 설정 */
    FIntPoint ParseResolution(const FString& ResolutionString) const;


public:
    virtual void NativeOnInitialized() override;
};


/** ================= 1. UPROPERTY (리플렉션 시스템 변수) ================= **/

/** ================= 2. UFUNCTION (리플렉션 시스템 함수) ================= **/

/** ================= 3. 생성자 및 필수 오버라이드 함수 ================= **/

/** ================= 4. 일반 메서드 (리플렉션이 필요 없는 함수) ================= **/

/** ================= 5. 일반 멤버 변수 (리플렉션이 필요 없는 변수) ================= **/
