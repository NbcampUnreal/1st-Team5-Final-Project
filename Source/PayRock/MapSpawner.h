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

public:	
	UPROPERTY(EditAnywhere, Category = "Modules")
	TArray<TSoftObjectPtr<UWorld>> ModuleLevels;

	UPROPERTY(EditAnywhere, Category = "Modules")
	float TileSize = 12550.0f;

	UPROPERTY(EditAnywhere, Category = "Modules")
	int32 GridSize = 2;
};