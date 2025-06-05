#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PatrolWithIdle.generated.h"

USTRUCT()
struct FPatrolIdleMemory
{
	GENERATED_BODY()

	float ElapsedTime;
	bool bWaitingForIdle;
};

UCLASS()
class PAYROCK_API UBTTask_PatrolWithIdle : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_PatrolWithIdle();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	virtual uint16 GetInstanceMemorySize() const override;

public:
	UPROPERTY(EditAnywhere, Category = "Patrol")
	float PatrolRadius = 1000.f;
	
	UPROPERTY(EditAnywhere, Category = "Patrol")
	float PatrolMoveSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	float IdleDuration = 2.f;
};