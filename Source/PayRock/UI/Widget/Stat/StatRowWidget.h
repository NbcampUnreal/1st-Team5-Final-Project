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

	// ;스텟 설명
	//void SetStatDescription(const FString& InDescription);
	void SetStatDescription(const FString& InDescription, const FText& InDisplayName);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatName;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatValue;

	// ;툴팁 UI
	UPROPERTY(EditDefaultsOnly, Category = "Stat")
	TSubclassOf<UUserWidget> TooltipWidgetClass;



private:
	FString StatDescription; // 설명 저장용 변수
	FText	StatDisplayName; // 한글(스텟으로)
};
