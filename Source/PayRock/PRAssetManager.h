// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "PRAssetManager.generated.h"

UCLASS()
class PAYROCK_API UPRAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UPRAssetManager& Get();

	bool bAssetsLoaded = false;

protected:
	virtual void StartInitialLoading() override;

	void OnLevelDataAssetLoaded();

	void OnVisualAssetsPreloaded(class ULevelDataAsset* PreloadAsset);
};
