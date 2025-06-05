// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseProgressBar.h"
#include "HealthBar.generated.h"
class UImage;
class UMaterialInterface;

UCLASS()
class PAYROCK_API UHealthBar : public UBaseProgressBar
{
	GENERATED_BODY()

public:
	virtual void OnWidgetControllerSet() override;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* HealthPercentText;


private:
	UPROPERTY()
	float Health;
	UPROPERTY()
	float MaxHealth;

	// ü�¹�-> �����̹����� �ٲ�
	UPROPERTY(meta = (BindWidget))
	UImage* HeartImage;


	// ���� �����Ϳ��� ������ �̹����� �� ������ ����
	// ��Ƽ���� �ν��Ͻ��� ������ ������
	UPROPERTY(EditAnywhere, Category = "Health Materials")
	UMaterialInterface* HeartMatFull;

	UPROPERTY(EditAnywhere, Category = "Health Materials")
	UMaterialInterface* HeartMat3Quarter;

	UPROPERTY(EditAnywhere, Category = "Health Materials")
	UMaterialInterface* HeartMatHalf;

	UPROPERTY(EditAnywhere, Category = "Health Materials")
	UMaterialInterface* HeartMatEmpty;


private:
	UFUNCTION()
	void OnHealthChanged(float NewHealth);
	UFUNCTION()
	void OnMaxHealthChanged(float NewMaxHealth);
	UFUNCTION()
	void UpdateHeartIcon();
};
