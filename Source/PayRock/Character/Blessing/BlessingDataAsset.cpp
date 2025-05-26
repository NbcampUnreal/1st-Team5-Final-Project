// PayRockGames


#include "BlessingDataAsset.h"


FBlessingData UBlessingDataAsset::GetNormalBlessingByNameAndType(const FText& Name, const EBlessingType& Type)
{
	switch (Type)
	{
	case EBlessingType::Passive:
		if (FBlessingData* DataPtr = PassiveBlessings.Find(Name.ToString()))
		{
			return *DataPtr;
		}
		break;
		
	case EBlessingType::Active:
		if (FBlessingData* DataPtr = ActiveBlessings.Find(Name.ToString()))
		{
			return *DataPtr;
		}
		break;
	}
	return FBlessingData();
}

void UBlessingDataAsset::SetBlessingRarity(FBlessingData& Blessing, EBlessingRarity Rarity)
{
	Blessing.BlessingRarity = Rarity;
}
