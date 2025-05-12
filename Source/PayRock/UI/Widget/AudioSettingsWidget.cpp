// PayRockGames


//#include "UI/Widget/AudioSettingsWidget.h"
#include "BaseUserWidget.h"
#include "AudioSettingsWidget.h"

#include "Components/Slider.h"
#include "Components/Button.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "Kismet/GameplayStatics.h"

void UAudioSettingsWidget::OnApplyClicked()
{
    ApplyAudioSettings();

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

    // �ʱⰪ ���� (�ɼ�)
    if (MasterVolumeSlider) MasterVolumeSlider->SetValue(1.0f);
    if (MusicVolumeSlider)  MusicVolumeSlider->SetValue(1.0f);
    if (SFXVolumeSlider)    SFXVolumeSlider->SetValue(1.0f);
}
