// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_UpdateClosestTarget.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTS_UpdateClosestTarget : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTS_UpdateClosestTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};