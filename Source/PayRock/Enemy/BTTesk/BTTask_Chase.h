#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTTask_Chase.generated.h"

UCLASS()
class PAYROCK_API UBTTask_Chase : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Chase();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

	UFUNCTION()
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
protected:
	UPROPERTY(EditAnywhere, Category = "Chase")
	float Speed = 600.f;

	UPROPERTY(EditAnywhere, Category = "Chase")
	float TargetRadius = 100.f;

private:
	UPROPERTY()
	class UBehaviorTreeComponent* CachedOwnerComp = nullptr;

	UPROPERTY()
	class AAIController* CachedAICon = nullptr;
};
