// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_ResetTargetActor.generated.h"

UCLASS()
class PAYROCK_API UBTS_ResetTargetActor : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTS_ResetTargetActor();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
