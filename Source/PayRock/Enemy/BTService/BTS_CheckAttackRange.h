// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTS_CheckAttackRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTS_CheckAttackRange();

	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRange;
};
