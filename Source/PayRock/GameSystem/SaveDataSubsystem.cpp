// PayRockGames


#include "SaveDataSubsystem.h"

void USaveDataSubsystem::SaveBlessing(const TArray<UBlessingDataAsset*>& BlessingArray)
{
	SavedBlessingsContainer.Empty();
	for (UBlessingDataAsset* Blessing : BlessingArray)
	{
		SavedBlessingsContainer.Add(Blessing);
	}
}
