// PayRockGames


#include "PRAssetManager.h"
#include "PRGameplayTags.h"
#include "GameSystem/LevelDataAsset.h"

UPRAssetManager& UPRAssetManager::Get()
{
	check(GEngine);
	return *Cast<UPRAssetManager>(GEngine->AssetManager);
}

void UPRAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FPRGameplayTags::InitializeNativeGameplayTags();

	FSoftObjectPath AssetPath(TEXT("/Game/PrimaryDataAssets/MainLevelData.MainLevelData"));
    
	GetStreamableManager().RequestAsyncLoad(
		AssetPath,
		FStreamableDelegate::CreateUObject(this, &UPRAssetManager::OnLevelDataAssetLoaded)
	);
}

void UPRAssetManager::OnLevelDataAssetLoaded()
{
	FSoftObjectPath AssetPath(TEXT("/Game/PrimaryDataAssets/MainLevelData.MainLevelData"));
	ULevelDataAsset* PreloadAsset = Cast<ULevelDataAsset>(AssetPath.ResolveObject());
	if (PreloadAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Asset loaded directly"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load asset directly"));
		return;
	}
	
	TArray<FSoftObjectPath> AssetsToStream;

	// Add all visual references
	for (const auto& Mesh : PreloadAsset->SkinnedMeshes)
		AssetsToStream.AddUnique(Mesh.ToSoftObjectPath());

	for (const auto& Mesh : PreloadAsset->StaticMeshes)
		AssetsToStream.AddUnique(Mesh.ToSoftObjectPath());

	for (const auto& Mat : PreloadAsset->Materials)
		AssetsToStream.AddUnique(Mat.ToSoftObjectPath());

	for (const auto& Tex : PreloadAsset->Textures)
		AssetsToStream.AddUnique(Tex.ToSoftObjectPath());

	for (const auto& Niagara : PreloadAsset->NiagaraSystems)
		AssetsToStream.AddUnique(Niagara.ToSoftObjectPath());

	if (PreloadAsset->LevelToOpen.IsValid())
		AssetsToStream.Add(PreloadAsset->LevelToOpen);

	GetStreamableManager().RequestAsyncLoad(
		AssetsToStream,
		FStreamableDelegate::CreateUObject(this, &UPRAssetManager::OnVisualAssetsPreloaded, PreloadAsset)
	);
}

void UPRAssetManager::OnVisualAssetsPreloaded(ULevelDataAsset* PreloadAsset)
{
	UE_LOG(LogTemp, Warning, TEXT("Visual Assets Preloaded"))
	// TODO : notify UI
	bAssetsLoaded = true;
}
