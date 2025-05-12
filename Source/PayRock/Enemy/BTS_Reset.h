// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_Reset.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTS_Reset : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTS_Reset();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
