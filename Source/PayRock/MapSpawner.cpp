// PayRockGames
#include "MapSpawner.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Kismet/GameplayStatics.h"

AMapSpawner::AMapSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMapSpawner::BeginPlay()
{
	Super::BeginPlay();	


    UWorld* World = GetWorld();
    if (!World) return;

    for (int x = 0; x < GridSize; ++x)
    {
        for (int y = 0; y < GridSize; ++y)
        {
            bool bSuccess = false;
            int32 Index = FMath::RandRange(0, ModuleLevels.Num() - 1);
            TSoftObjectPtr<UWorld> LevelToLoad = ModuleLevels[Index];

            FVector Location(x * TileSize, y * TileSize, 0);
            FName UniqueName = FName(*FString::Printf(TEXT("Module_%d_%d"), x, y));

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
        }
    }
}
