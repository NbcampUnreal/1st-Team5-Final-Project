// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ReturnToStart.generated.h"


UCLASS()
class PAYROCK_API UBTTask_ReturnToStart : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_ReturnToStart();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};