// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_CheckPlayerDeath_Stealth.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTS_CheckPlayerDeath_Stealth : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTS_CheckPlayerDeath_Stealth();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};