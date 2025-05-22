// PayRockGames

#include "SaveDataSubsystem.h"

void USaveDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FSoftObjectPath DataAssetPath(TEXT("/Game/Character/Blessing/Data/DA_BlessingDataAsset"));
	if (UBlessingDataAsset* LoadedDataAsset = Cast<UBlessingDataAsset>(DataAssetPath.TryLoad()))
	{
		BlessingDataAsset = LoadedDataAsset;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load BlessingDataAsset!"));
	}
}

const UBlessingDataAsset* USaveDataSubsystem::GetBlessingDataAsset() const
{
	return BlessingDataAsset;
}

void USaveDataSubsystem::SaveBlessing(const FBlessingData& Blessing)
{
	SavedBlessingsContainer.Add(Blessing);
}
