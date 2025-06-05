// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapSpawner.generated.h"

UCLASS()
class PAYROCK_API AMapSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AMapSpawner();

protected:
	virtual void BeginPlay() override;
	void SpawnOnServer();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StartModuleStreaming(const TArray<int32>& InModuleIndexes);

	void StreamNextModule();

public:	
	UPROPERTY(EditAnywhere, Category = "Modules")
	TArray<TSoftObjectPtr<UWorld>> ModuleLevels;

	UPROPERTY(EditAnywhere, Category = "Modules")
	float TileSize = 12550.0f;

	UPROPERTY(EditAnywhere, Category = "Modules")
	int32 GridSize = 2;

private:
	UPROPERTY()
	TArray<int32> SelectedModuleIndexes;

	UPROPERTY()
	TArray<int32> StreamingIndexes;

	UPROPERTY()
	int32 NextSpawnIndex;

	/*UFUNCTION(NetMulticast, Reliable)
	void Multicast_SpawnModules(const TArray<int32>& InModuleIndexes);*/
};