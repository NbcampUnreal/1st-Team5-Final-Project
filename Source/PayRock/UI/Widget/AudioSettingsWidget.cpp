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
    // MainMenu 다시 보여주기
    if (MainMenuRef)  //; 체크 
    {
        MainMenuRef->SetVisibility(ESlateVisibility::Visible);

        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        if (PC)
        {
            //; 인풋모드 변경 
            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(MainMenuRef->TakeWidget());
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
        }


        //  이제는 직접 참조해서 숨기기 -> 구조가 switcher 구조라서..
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
