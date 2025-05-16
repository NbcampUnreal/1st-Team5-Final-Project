#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PatrolWithIdle.generated.h"

USTRUCT()
struct FPatrolIdleMemory
{
	GENERATED_BODY()

	float ElapsedTime = 0.f;
	bool bWaitingForIdle = false;
};

UCLASS()
class PAYROCK_API UBTTask_PatrolWithIdle : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_PatrolWithIdle();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override {}

	virtual uint16 GetInstanceMemorySize() const override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	float PatrolRadius = 500.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float IdleDuration = 2.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float PatrolMoveSpeed = 200.f;
};
