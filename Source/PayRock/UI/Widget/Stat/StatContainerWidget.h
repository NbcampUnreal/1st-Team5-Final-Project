// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/UI/Widget/BaseUserWidget.h"
#include "StatContainerWidget.generated.h"

class UScrollBox;
class UVerticalBox;

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
