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
	virtual uint16 GetInstanceMemorySize() const override;

protected:
	FVector StartPosition;
	float ElapsedTime = 0.f;
	bool bShouldReturn = false;

	UPROPERTY(EditAnywhere, Category = "Idle")
	float WaitDuration = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Idle")
	float MaxDistanceFromPlayer = 500.0f;
	

	UBehaviorTreeComponent* CachedOwnerComp = nullptr;
};