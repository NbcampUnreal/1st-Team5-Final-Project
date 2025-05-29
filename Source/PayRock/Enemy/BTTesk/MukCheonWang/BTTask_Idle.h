// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Idle.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTTask_Idle : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_Idle();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Idle")
	float IdleTime = 1.0f;

	FTimerHandle IdleTimerHandle;

	void FinishIdle(UBehaviorTreeComponent* OwnerComp);
};