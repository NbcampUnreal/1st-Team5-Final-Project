// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_KappaIdle.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTTask_KappaIdle : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_KappaIdle();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Kappa|Idle")
	float MaxDistanceFromPlayer = 800.0f;

	UPROPERTY(EditAnywhere, Category = "Kappa|Idle")
	float WaitDuration = 3.0f;

private:
	FVector StartPosition;
	FTimerHandle WaitTimerHandle;
	bool bShouldReturn = false;

	UBehaviorTreeComponent* CachedOwnerComp;
};

