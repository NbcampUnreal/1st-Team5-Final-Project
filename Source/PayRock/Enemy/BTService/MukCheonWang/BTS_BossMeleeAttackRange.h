// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_BossMeleeAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTS_BossMeleeAttackRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTS_BossMeleeAttackRange();

protected:
	UPROPERTY(EditAnywhere, Category = "Range")
	float MeleeRange = 180.f;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};