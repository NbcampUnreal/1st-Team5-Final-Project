// PayRockGames

#pragma once

#include "CoreMinimal.h"
//#include "UI/Widget/BaseUserWidget.h"
#include "BaseUserWidget.h"
#include "OptionsMenuWidget.generated.h"

/**
 *  옵션 메뉴 메인 위젯 (탭 전환 포함)
 */

class UButton;
class UMainMenuUserWidget;
class UWidgetSwitcher;
class UVideoSettingsWidget;
class UAudioSettingsWidget;
// 등등


UCLASS()
class PAYROCK_API UOptionsMenuWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
protected:
    // 탭 전환용 버튼들
    UPROPERTY(meta = (BindWidget))
    UButton* VideoButton;

    UPROPERTY(meta = (BindWidget))
    UButton* AudioButton;

    UPROPERTY(meta = (BindWidget))
    UButton* InputButton;

    UPROPERTY(meta = (BindWidget))
    UButton* GameplayButton;

    // 탭 콘텐츠 전환용 위젯 스위처
    UPROPERTY(meta = (BindWidget))
    UWidgetSwitcher* TabSwitcher;

public:
    // 메인메뉴 기억하기 위해서(back)
    UPROPERTY()
    UMainMenuUserWidget* MainMenuRef;

    UPROPERTY(meta = (BindWidget))
    UVideoSettingsWidget* VideoSettingsWidget;

    UPROPERTY(meta = (BindWidget))
    UAudioSettingsWidget* AudioSettingsWidget;


protected:
    // 클릭 핸들러들
    UFUNCTION()
    void OnVideoButtonClicked();

    UFUNCTION()
    void OnAudioButtonClicked();

    UFUNCTION()
    void OnInputButtonClicked();

    UFUNCTION()
    void OnGameplayButtonClicked();

    // 인덱스에 따라서 옵션 탭 바뀜 
    void SwitchTab(int32 TabIndex);


public:
	virtual void NativeOnInitialized() override;

    // 커스텀 이닛
    void InitOptionsMenu(UMainMenuUserWidget* InMainMenuRef);
    

};


/** ================= 1. UPROPERTY (리플렉션 시스템 변수) ================= **/

/** ================= 2. UFUNCTION (리플렉션 시스템 함수) ================= **/

/** ================= 3. 생성자 및 필수 오버라이드 함수 ================= **/

/** ================= 4. 일반 메서드 (리플렉션이 필요 없는 함수) ================= **/

/** ================= 5. 일반 멤버 변수 (리플렉션이 필요 없는 변수) ================= **/
