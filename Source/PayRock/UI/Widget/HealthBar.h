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
	class UProgressBar* HealthBar;


private:
	UPROPERTY()
	float Health;
	UPROPERTY()
	float MaxHealth;

	// 체력바-> 심장이미지로 바꿈
	UPROPERTY(meta = (BindWidget))
	UImage* HeartImage;


	// 위젯 에디터에서 각각의 이미지를 이 변수에 연결
	// 머티리얼 인스턴스를 연결할 변수들
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
