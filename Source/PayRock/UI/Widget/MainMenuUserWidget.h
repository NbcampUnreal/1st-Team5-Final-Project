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
    /** ���� ���� ��ư */
    UPROPERTY(meta = (BindWidget))
    UButton* StartGameButton;

    /** ���� ���� ��ư */
    UPROPERTY(meta = (BindWidget))
    UButton* QuitGameButton;

    /** �ɼ� ��ư - �ʿ� �� ��� */
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


/** ================= 1. UPROPERTY (���÷��� �ý��� ����) ================= **/

/** ================= 2. UFUNCTION (���÷��� �ý��� �Լ�) ================= **/

/** ================= 3. ������ �� �ʼ� �������̵� �Լ� ================= **/

/** ================= 4. �Ϲ� �޼��� (���÷����� �ʿ� ���� �Լ�) ================= **/

/** ================= 5. �Ϲ� ��� ���� (���÷����� �ʿ� ���� ����) ================= **/
