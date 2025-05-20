// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Enemy/EnemyController.h"
#include "MarketClownMonsterController.generated.h"

UCLASS()
class PAYROCK_API AMarketClownMonsterController : public AEnemyController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMarketClownMonsterController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
