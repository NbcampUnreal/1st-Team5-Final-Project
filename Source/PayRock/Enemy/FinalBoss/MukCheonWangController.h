// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PayRock/Enemy/EnemyController.h"
#include "MukCheonWangController.generated.h"

UCLASS()
class PAYROCK_API AMukCheonWangController : public AAIController
{
	GENERATED_BODY()

public:
	AMukCheonWangController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;
};