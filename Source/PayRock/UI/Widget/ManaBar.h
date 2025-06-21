// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseProgressBar.h"
#include "ManaBar.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UManaBar : public UBaseProgressBar
{
	GENERATED_BODY()

public:
	virtual void OnWidgetControllerSet() override;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ManaBar;

private:
	UFUNCTION()
	void OnManaChanged(float NewMana);
	UFUNCTION()
	void OnMaxManaChanged(float NewMaxMana);
	UFUNCTION()
	void FadeOut();

	UPROPERTY()
	float Mana;
	UPROPERTY()
	float MaxMana;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeInAnim;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeOutAnim;
	UPROPERTY(EditDefaultsOnly, Category = "Fade")
	float DisplayDuration = 7.5f;

	FTimerHandle FadeOutTimer;

	bool bIsDisplayed = false;
};
