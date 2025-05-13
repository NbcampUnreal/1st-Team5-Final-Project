// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Chase.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTTask_Chase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_Chase();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
