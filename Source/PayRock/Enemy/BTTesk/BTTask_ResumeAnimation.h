// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ResumeAnimation.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTTask_ResumeAnimation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_ResumeAnimation();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
