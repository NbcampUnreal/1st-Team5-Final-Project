// PayRockGames

#pragma once

#include "CoreMinimal.h"
//#include "UI/Widget/BaseUserWidget.h"
#include "BaseUserWidget.h"
#include "MainMenuUserWidget.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class PAYROCK_API UMainMenuUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
protected:
    /** 게임 시작 버튼 */
    UPROPERTY(meta = (BindWidget))
    UButton* StartGameButton;

    /** 게임 종료 버튼 */
    UPROPERTY(meta = (BindWidget))
    UButton* QuitGameButton;

    /** 옵션 버튼 - 필요 시 사용 */
    UPROPERTY(meta = (BindWidgetOptional))
    UButton* OptionsButton;

private:
    UFUNCTION()
    void HandleStartGameClicked();

    UFUNCTION()
    void HandleQuitGameClicked();

    UFUNCTION()
    void HandleOptionsClicked();


public:
    virtual void NativeOnInitialized() override;

};


/** ================= 1. UPROPERTY (리플렉션 시스템 변수) ================= **/

/** ================= 2. UFUNCTION (리플렉션 시스템 함수) ================= **/

/** ================= 3. 생성자 및 필수 오버라이드 함수 ================= **/

/** ================= 4. 일반 메서드 (리플렉션이 필요 없는 함수) ================= **/

/** ================= 5. 일반 멤버 변수 (리플렉션이 필요 없는 변수) ================= **/
