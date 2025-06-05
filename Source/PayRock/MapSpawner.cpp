// PayRockGames
#include "MapSpawner.h"

#include "NavigationSystem.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AMapSpawner::AMapSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;
}

void AMapSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMapSpawner::SpawnOnServer, 10.0f, false);
	}
}

void AMapSpawner::SpawnOnServer()
{
	SelectedModuleIndexes.Empty();

	for (int x = 0; x < GridSize; ++x)
	{
		for (int y = 0; y < GridSize; ++y)
		{
			int32 Index = FMath::RandRange(0, ModuleLevels.Num() - 1);
			SelectedModuleIndexes.Add(Index);
		}
	}
	NextSpawnIndex = 0;
	Multicast_StartModuleStreaming(SelectedModuleIndexes);
}

void AMapSpawner::Multicast_StartModuleStreaming_Implementation(const TArray<int32>& InModuleIndexes)
{
	StreamingIndexes = InModuleIndexes;
	NextSpawnIndex = 0;

	StreamNextModule();
}

void AMapSpawner::StreamNextModule()
{
	if (!StreamingIndexes.IsValidIndex(NextSpawnIndex)) return;

	int x = NextSpawnIndex / GridSize;
	int y = NextSpawnIndex % GridSize;

	TSoftObjectPtr<UWorld> LevelToLoad = ModuleLevels[StreamingIndexes[NextSpawnIndex]];
	FVector Location = FVector(x * TileSize, y * TileSize, 0);
	FName BaseName = FName(*FString::Printf(TEXT("Module_%d_%d"), x, y));
	FName UniqueName = MakeUniqueObjectName(GetWorld(), ULevelStreamingDynamic::StaticClass(), BaseName);

	bool bSuccess = false;
	ULevelStreamingDynamic* StreamingLevel = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
		this,
		LevelToLoad,
		Location,
		FRotator::ZeroRotator,
		bSuccess
	);

	if (!bSuccess)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load level: %s"), *LevelToLoad.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Streamed module (%d, %d): %s"), x, y, *LevelToLoad.ToString());
	}

	NextSpawnIndex++;

	if (StreamingIndexes.IsValidIndex(NextSpawnIndex))
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMapSpawner::StreamNextModule, 4.0f, false);
	}
	else
	{
		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if (NavSys)
		{
			NavSys->OnNavigationBoundsUpdated(nullptr);
			UE_LOG(LogTemp, Log, TEXT("NavMesh updated after streaming all modules"));
		}
	}
}

/*
void AMapSpawner::Multicast_SpawnModules_Implementation(const TArray<int32>& InModuleIndexes)
{
	UWorld* World = GetWorld();
	if (!World) return;

	int Index = 0;

	for (int x = 0; x < GridSize; ++x)
	{
		for (int y = 0; y < GridSize; ++y)
		{
			if (!InModuleIndexes.IsValidIndex(Index)) continue;

			TSoftObjectPtr<UWorld> LevelToLoad = ModuleLevels[InModuleIndexes[Index]];
			FVector Location(x * TileSize, y * TileSize, 0);
			FName UniqueName = FName(*FString::Printf(TEXT("Module_%d_%d"), x, y));
			bool bSuccess = false;
			ULevelStreamingDynamic* StreamingLevel = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
				this,
				LevelToLoad,
				Location,
				FRotator::ZeroRotator,
				bSuccess
			);

			if (!bSuccess)
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to load level: %s"), *LevelToLoad.ToString());
			}
			++Index;
		}
	}
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSys)
	{
		NavSys->OnNavigationBoundsUpdated(nullptr);
	}
}*/
