// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TranslateDataAsset.generated.h"

UCLASS()
class PAYROCK_API UTranslateDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI|Translation")
	FString TranslateEnglishToKorean(const FString& InString);
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "UI|Translation")
	TMap<FString, FString> EnglishToKorean;
};
