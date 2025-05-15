// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PauseAnimation.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTTask_PauseAnimation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_PauseAnimation();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
};
