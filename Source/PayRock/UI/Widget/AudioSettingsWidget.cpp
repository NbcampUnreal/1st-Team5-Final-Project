// PayRockGames


//#include "UI/Widget/AudioSettingsWidget.h"
#include "AudioSettingsWidget.h"
#include "BaseUserWidget.h"
#include "MainMenuUserWidget.h"
#include "OptionsMenuWidget.h"

#include "Components/Slider.h"
#include "Components/Button.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "Kismet/GameplayStatics.h"

void UAudioSettingsWidget::OnApplyClicked()
{
    ApplyAudioSettings();

}

void UAudioSettingsWidget::OnBackClicked()
{
    // MainMenu �ٽ� �����ֱ�
    if (MainMenuRef)  //; üũ 
    {
        MainMenuRef->SetVisibility(ESlateVisibility::Visible);

        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        if (PC)
        {
            //; ��ǲ��� ���� 
            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(MainMenuRef->TakeWidget());
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
        }


        //  ������ ���� �����ؼ� ����� -> ������ switcher ������..
        if (OptionsMenuRef)
        {
            OptionsMenuRef->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void UAudioSettingsWidget::ApplyAudioSettings()
{
    if (!AudioMix) return;

    float MasterVol = MasterVolumeSlider ? MasterVolumeSlider->GetValue() : 1.0f;
    float MusicVol = MusicVolumeSlider ? MusicVolumeSlider->GetValue() : 1.0f;
    float SFXVol = SFXVolumeSlider ? SFXVolumeSlider->GetValue() : 1.0f;

    // ���� �ͽ��� Ǫ���ؾ� �������̵尡 �۵���
    UGameplayStatics::PushSoundMixModifier(this, AudioMix);

    if (MasterSoundClass)
    {
        UGameplayStatics::SetSoundMixClassOverride(this, AudioMix, MasterSoundClass, MasterVol, 1.0f, 0.0f, true);
    }
    if (MusicSoundClass)
    {
        UGameplayStatics::SetSoundMixClassOverride(this, AudioMix, MusicSoundClass, MusicVol, 1.0f, 0.0f, true);
    }
    if (SFXSoundClass)
    {
        UGameplayStatics::SetSoundMixClassOverride(this, AudioMix, SFXSoundClass, SFXVol, 1.0f, 0.0f, true);
    }
}

void UAudioSettingsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ApplyButton)
    {
        ApplyButton->OnClicked.AddDynamic(this, &UAudioSettingsWidget::OnApplyClicked);
    }

    if (BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &UAudioSettingsWidget::OnBackClicked);

    }

    // �ʱⰪ ���� (�ɼ�)
    if (MasterVolumeSlider) MasterVolumeSlider->SetValue(1.0f);
    if (MusicVolumeSlider)  MusicVolumeSlider->SetValue(1.0f);
    if (SFXVolumeSlider)    SFXVolumeSlider->SetValue(1.0f);
}
