// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_BossChase.generated.h"


UCLASS()
class PAYROCK_API UBTTask_BossChase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_BossChase();
	
private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
