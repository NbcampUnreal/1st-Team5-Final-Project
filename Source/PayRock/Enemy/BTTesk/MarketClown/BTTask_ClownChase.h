// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ClownChase.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTTask_ClownChase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_ClownChase();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	UPROPERTY()
	class AAIController* CachedAICon;

	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp;

	UPROPERTY()
	AActor* CachedTarget;

	UPROPERTY(EditAnywhere, Category = "Chase")
	float Speed;

	UPROPERTY(EditAnywhere, Category = "Chase")
	float TargetRadius;
};