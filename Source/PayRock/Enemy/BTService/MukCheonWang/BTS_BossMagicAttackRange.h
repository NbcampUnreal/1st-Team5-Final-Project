// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_BossMagicAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTS_BossMagicAttackRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTS_BossMagicAttackRange();

protected:
	UPROPERTY(EditAnywhere, Category = "Range")
	float MagicRange = 1200.f;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};