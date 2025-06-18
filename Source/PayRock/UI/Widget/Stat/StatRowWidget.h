// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatRowWidget.generated.h"

class UTextBlock;

UCLASS(meta = (DisableNativeTick))
class PAYROCK_API UStatRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetStatName(const FString& Name);


	void SetStatValue(const float& Value);

	// ;���� ����
	//void SetStatDescription(const FString& InDescription);
	void SetStatDescription(const FString& InDescription, const FText& InDisplayName);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatName;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatValue;

	// ;���� UI
	UPROPERTY(EditDefaultsOnly, Category = "Stat")
	TSubclassOf<UUserWidget> TooltipWidgetClass;



private:
	FString StatDescription; // ���� ����� ����
	FText	StatDisplayName; // �ѱ�(��������)
};
