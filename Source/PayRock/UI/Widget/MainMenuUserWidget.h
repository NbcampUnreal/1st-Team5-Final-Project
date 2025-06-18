// PayRockGames

#pragma once

#include "CoreMinimal.h"
//#include "UI/Widget/BaseUserWidget.h"
#include "BaseUserWidget.h"
#include "MainMenuUserWidget.generated.h"

class UButton;
class UOptionsMenuWidget;
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

    /** �ȳ� ��ư - �ʿ� �� ��� */
    UPROPERTY(meta = (BindWidgetOptional))
    UButton* HelpButton;

    /** ũ���� ��ư - �ʿ� �� ��� */
    UPROPERTY(meta = (BindWidgetOptional))
    UButton* CreditButton;


    //UPROPERTY(EditAnywhere, Category = "UI")
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UOptionsMenuWidget> OptionsMenuWidgetClass;

    //// WBP_HelpWidget�� Ŭ������ ������ �� �ֵ���
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    //TSubclassOf<UUserWidget> HelpWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UHelpWidgetMenu> HelpWidgetClass;

private:
    UFUNCTION()
    void HandleStartGameClicked();

    UFUNCTION()
    void HandleQuitGameClicked();

    UFUNCTION()
    void HandleOptionsClicked();

    UFUNCTION()
    void HandleHelpClicked();


public:
    virtual void NativeOnInitialized() override;

};


/** ================= 1. UPROPERTY (���÷��� �ý��� ����) ================= **/

/** ================= 2. UFUNCTION (���÷��� �ý��� �Լ�) ================= **/

/** ================= 3. ������ �� �ʼ� �������̵� �Լ� ================= **/

/** ================= 4. �Ϲ� �޼��� (���÷����� �ʿ� ���� �Լ�) ================= **/

/** ================= 5. �Ϲ� ��� ���� (���÷����� �ʿ� ���� ����) ================= **/
