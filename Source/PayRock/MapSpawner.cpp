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
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMapSpawner::SpawnOnServer, 15.0f, false);
	}
}

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
	Multicast_SpawnModules(SelectedModuleIndexes);
}