#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ReturnToStart.generated.h"

UCLASS()
class PAYROCK_API UBTTask_ReturnToStart : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ReturnToStart();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
