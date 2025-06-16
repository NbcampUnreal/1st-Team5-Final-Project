// PayRockGames


#include "BuffDataAsset.h"

FPrimaryAssetId UBuffDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("BuffDataAsset", GetFName());
}

UAnimMontage* UBuffDataAsset::GetReactMontageByTag(const FGameplayTag& Tag) const
{
	if (ReactMontages.Contains(Tag))
	{
		return *ReactMontages.Find(Tag);
	}
	return nullptr;
}


