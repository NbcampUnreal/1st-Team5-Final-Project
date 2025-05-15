// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_CheckPlayerDetect.generated.h"


UCLASS()
class PAYROCK_API UBTS_CheckPlayerDetect : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTS_CheckPlayerDetect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

