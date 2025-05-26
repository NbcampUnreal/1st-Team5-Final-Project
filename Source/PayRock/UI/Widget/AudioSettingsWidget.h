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
class UMainMenuUserWidget;
class UOptionsMenuWidget;

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

    /** �����Ϳ��� ���� ������ ���� ���µ� */
    UPROPERTY(EditDefaultsOnly, Category = "Audio")
    USoundMix* AudioMix;

    UPROPERTY(EditDefaultsOnly, Category = "Audio")
    USoundClass* MasterSoundClass;

    UPROPERTY(EditDefaultsOnly, Category = "Audio")
    USoundClass* MusicSoundClass;

    UPROPERTY(EditDefaultsOnly, Category = "Audio")
    USoundClass* SFXSoundClass;

public:
    // MainMenu ������ ���� ���� (SetVisibility�� ���� �ʿ�)
    UPROPERTY()
    UMainMenuUserWidget* MainMenuRef;

    UPROPERTY()
    UOptionsMenuWidget* OptionsMenuRef;

    UFUNCTION()
    void OnApplyClicked();

    /** �ڷ� ���� ��ư **/
    UFUNCTION()
    void OnBackClicked();

    void ApplyAudioSettings();

public:
    virtual void NativeOnInitialized() override;
};
