// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Enemy/EnemyController.h"
#include "MarketClownMonsterController.generated.h"

UENUM(BlueprintType)
enum class EClownStrategyType : uint8
{
	Defensive UMETA(DisplayName = "Defensive"),
	Aggressive UMETA(DisplayName = "Aggressive"),
	Berserk   UMETA(DisplayName = "Berserk"),
	Ranged    UMETA(DisplayName = "Ranged")
};

UCLASS()
class PAYROCK_API AMarketClownMonsterController : public AEnemyController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn) override;

	
	UBehaviorTree* GetBehaviorTree(){ return DefaultBehaviorTree; };

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName StrategyKeyName = "StrategyType";
};