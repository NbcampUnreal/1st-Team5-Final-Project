// PayRockGames

#pragma once

#include "CoreMinimal.h"
//#include "UI/Widget/BaseUserWidget.h"
#include "BaseUserWidget.h"
#include "AudioSettingsWidget.generated.h"


class USlider;
class UButton;
class USoundMix;
class USoundClass;
/**
 * 
 */
UCLASS()
class PAYROCK_API UAudioSettingsWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(meta = (BindWidget))
    USlider* MasterVolumeSlider;

    UPROPERTY(meta = (BindWidget))
    USlider* MusicVolumeSlider;

    UPROPERTY(meta = (BindWidget))
    USlider* SFXVolumeSlider;

    UPROPERTY(meta = (BindWidget))
    UButton* ApplyButton;

    UPROPERTY(meta = (BindWidget))
    UButton* BackButton;

    /** 에디터에서 설정 가능한 사운드 에셋들 */
    UPROPERTY(EditDefaultsOnly, Category = "Audio")
    USoundMix* AudioMix;

    UPROPERTY(EditDefaultsOnly, Category = "Audio")
    USoundClass* MasterSoundClass;

    UPROPERTY(EditDefaultsOnly, Category = "Audio")
    USoundClass* MusicSoundClass;

    UPROPERTY(EditDefaultsOnly, Category = "Audio")
    USoundClass* SFXSoundClass;

    UFUNCTION()
    void OnApplyClicked();

    /** 뒤로 가기 버튼 **/
    UFUNCTION()
    void OnBackClicked();

    void ApplyAudioSettings();

public:
    virtual void NativeOnInitialized() override;
};
