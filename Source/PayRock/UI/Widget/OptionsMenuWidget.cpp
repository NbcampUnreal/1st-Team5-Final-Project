// PayRockGames


//#include "UI/Widget/OptionsMenuWidget.h"
#include "BaseUserWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "OptionsMenuWidget.h"

void UOptionsMenuWidget::OnVideoButtonClicked()
{
    SwitchTab(0);

}

void UOptionsMenuWidget::OnAudioButtonClicked()
{
    SwitchTab(1);

}

void UOptionsMenuWidget::OnInputButtonClicked()
{
    SwitchTab(2);

}

void UOptionsMenuWidget::OnGameplayButtonClicked()
{
    SwitchTab(3);

}

void UOptionsMenuWidget::SwitchTab(int32 TabIndex)
{
    // �� ����ó�� ��ȿ����(��������) && TabIndex�� ��ȿ����(ex) 4���� �����̸� 0,1,2,3 �� ��ȿ�ϴ�.
    if (TabSwitcher && TabSwitcher->GetNumWidgets(/*�𸮾������Լ�*/) > TabIndex)
    {
        // �ɼ� �� �ٲٱ� 
        TabSwitcher->/*UE����*/SetActiveWidgetIndex(TabIndex);
    }
}

void UOptionsMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    // ; �Լ��� ����
    if (VideoButton)
    {
        VideoButton->OnClicked.AddDynamic(this, &UOptionsMenuWidget::OnVideoButtonClicked);
    }

    if (AudioButton)
    {
        AudioButton->OnClicked.AddDynamic(this, &UOptionsMenuWidget::OnAudioButtonClicked);
    }

    if (InputButton)
    {
        InputButton->OnClicked.AddDynamic(this, &UOptionsMenuWidget::OnInputButtonClicked);
    }

    if (GameplayButton)
    {
        GameplayButton->OnClicked.AddDynamic(this, &UOptionsMenuWidget::OnGameplayButtonClicked);
    }

    // �⺻ �� ���� (���� ��)
    SwitchTab(0);
}
