// PayRockGames


#include "PRAssetManager.h"
#include "PRGameplayTags.h"

UPRAssetManager& UPRAssetManager::Get()
{
	check(GEngine);
	return *Cast<UPRAssetManager>(GEngine->AssetManager);
}

void UPRAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FPRGameplayTags::InitializeNativeGameplayTags();
}
