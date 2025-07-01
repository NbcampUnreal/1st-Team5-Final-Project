// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PlayRangeAttackMontage.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTTask_PlayRangeAttackMontage : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_PlayRangeAttackMontage();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* RangeAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FName BlackboardKey_RangeAttackReady = "bInRangeAttackRange";

private:
	bool bIsMontagePlaying = false;
	float MontageDuration = 0.f;
	float ElapsedTime = 0.f;
};