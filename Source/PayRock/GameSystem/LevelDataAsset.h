// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Engine/DataAsset.h"
#include "LevelDataAsset.generated.h"

UCLASS(Blueprintable)
class PAYROCK_API ULevelDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(FPrimaryAssetType(FName(TEXT("LevelDataAsset"))), FName(TEXT("MainLevelData")));
	}
	
	UPROPERTY(EditDefaultsOnly, Category = "Visuals", meta = (AllowedClasses = "/Script/Engine.World"))
	FSoftObjectPath LevelToOpen;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TArray<TSoftObjectPtr<USkeletalMesh>> SkinnedMeshes;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TArray<TSoftObjectPtr<UStaticMesh>> StaticMeshes;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TArray<TSoftObjectPtr<UMaterialInterface>> Materials;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TArray<TSoftObjectPtr<UTexture>> Textures;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TArray<TSoftObjectPtr<UNiagaraSystem>> NiagaraSystems;

#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = "Asset Building")
	void BuildAssetListFromLevel();
	UFUNCTION()
	void OnLevelLoaded(const FSoftObjectPath& LoadedPath, UObject* LoadedObject);
#endif
};
