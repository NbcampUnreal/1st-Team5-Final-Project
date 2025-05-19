// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseProgressBar.h"
#include "HealthBar.generated.h"


UCLASS()
class PAYROCK_API UHealthBar : public UBaseProgressBar
{
	GENERATED_BODY()

public:
	virtual void OnWidgetControllerSet() override;

	UPROPERTY(meta=(BindWidget))
	class UProgressBar* HealthBar;

private:
	UFUNCTION()
	void OnHealthChanged(float NewHealth);
	UFUNCTION()
	void OnMaxHealthChanged(float NewMaxHealth);

	UPROPERTY()
	float Health;
	UPROPERTY()
	float MaxHealth;
};
