// PayRockGames

#include "SaveDataSubsystem.h"
#include "GameFramework/GameUserSettings.h"

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

	EquippedActiveBlessing.BlessingType = EBlessingType::Active;
	EquippedPassiveBlessing.BlessingType = EBlessingType::Passive;

	/**
	 * TEST
	 */
	for (const auto Blessing : BlessingDataAsset->ActiveBlessings)
	{
		SaveBlessing(Blessing.Value);
	}
	for (const auto Blessing : BlessingDataAsset->PassiveBlessings)
	{
		SaveBlessing(Blessing.Value);
	}

	UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
	if (UserSettings)
	{
		UserSettings->SetScreenResolution(FIntPoint(1280, 720));

		UserSettings->SetFullscreenMode(EWindowMode::Windowed);

		UserSettings->SetOverallScalabilityLevel(0);
		UserSettings->SetVisualEffectQuality(2);

		UserSettings->ApplySettings(false);
		UserSettings->SaveSettings();
	}
}

const UBlessingDataAsset* USaveDataSubsystem::GetBlessingDataAsset() const
{
	return BlessingDataAsset;
}

void USaveDataSubsystem::SaveBlessing(const FBlessingData& Blessing)
{
	SavedBlessingsContainer.Add(Blessing);
	OnBlessingSaved.Broadcast(SavedBlessingsContainer);
}
