// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTTask_PatrolWithIdle.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTTask_PatrolWithIdle : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_PatrolWithIdle();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

protected:
	void StartIdleDelay();

	UBehaviorTreeComponent* CachedOwnerComp = nullptr;
	FTimerHandle IdleTimerHandle;

	UPROPERTY(EditAnywhere, Category = "AI")
	float PatrolRadius = 800.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float IdleDuration = 4.0f;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	float PatrolMoveSpeed = 200.0f;

	bool bWaitingForIdle = false;
	bool bReachedDestination = false;
};
