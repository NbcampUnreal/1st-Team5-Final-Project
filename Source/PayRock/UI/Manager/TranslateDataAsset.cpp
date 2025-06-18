// PayRockGames


#include "TranslateDataAsset.h"

FString UTranslateDataAsset::TranslateEnglishToKorean(const FString& InString)
{
	if (FString* StringPtr = EnglishToKorean.Find(InString))
	{
		return *StringPtr;
	}
	return FString("");
}
