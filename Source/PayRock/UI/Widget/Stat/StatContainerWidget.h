// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/UI/Widget/BaseUserWidget.h"
#include "StatContainerWidget.generated.h"

class UScrollBox;
class UVerticalBox;
class UStatInfoDataAsset;

UCLASS(meta = (DisableNativeTick))
class PAYROCK_API UStatContainerWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	void InitializeStatRows();
	
protected:
	virtual void OnWidgetControllerSet() override;

	UFUNCTION()
	void UpdateStatRow(const FString& StatName, const float& StatValue);

public:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* PrimaryStatBox;
	UPROPERTY(meta = (BindWidget))
	UScrollBox* SecondaryStatBox;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Stat")
	TSubclassOf<UUserWidget> StatRowWidgetClass;
	UPROPERTY()
	TMap<FString, UUserWidget*> StatNameRowMap;

	// ;Stat 이름과 설명을 매핑하는 맵
	UPROPERTY()
	TMap<FString, FString> StatDescriptions;

	// ;데이터 에셋 참조
	UPROPERTY(EditDefaultsOnly, Category = "Stat")
	UStatInfoDataAsset* StatInfoDataAsset;

	/*
	* TODO: This is STUPID!!! Separate Primary / Secondary using GameplayTag?
	*/
	UPROPERTY()
	TArray<FString> PrimaryStatNames = {
		FString(TEXT("Strength")),
		FString(TEXT("Intelligence")),
		FString(TEXT("Dexterity")),
		FString(TEXT("Vitality")),
		FString(TEXT("Spirit")),
		FString(TEXT("Luck"))
	};
};
