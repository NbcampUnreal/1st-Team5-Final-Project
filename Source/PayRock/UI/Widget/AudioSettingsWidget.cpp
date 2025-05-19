// PayRockGames


//#include "UI/Widget/AudioSettingsWidget.h"
#include "AudioSettingsWidget.h"
#include "BaseUserWidget.h"

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
    // 다시 메인 메뉴로 

    RemoveFromParent(); // 단순 닫기

    // 또는 이벤트 디스패치
    // OnBackRequested(); ← Blueprint에서 연결해서 다음 화면 전환 처리
}

void UAudioSettingsWidget::ApplyAudioSettings()
{
    if (!AudioMix) return;

    float MasterVol = MasterVolumeSlider ? MasterVolumeSlider->GetValue() : 1.0f;
    float MusicVol = MusicVolumeSlider ? MusicVolumeSlider->GetValue() : 1.0f;
    float SFXVol = SFXVolumeSlider ? SFXVolumeSlider->GetValue() : 1.0f;

    // 사운드 믹스를 푸시해야 오버라이드가 작동함
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

    // 초기값 세팅 (옵션)
    if (MasterVolumeSlider) MasterVolumeSlider->SetValue(1.0f);
    if (MusicVolumeSlider)  MusicVolumeSlider->SetValue(1.0f);
    if (SFXVolumeSlider)    SFXVolumeSlider->SetValue(1.0f);
}
