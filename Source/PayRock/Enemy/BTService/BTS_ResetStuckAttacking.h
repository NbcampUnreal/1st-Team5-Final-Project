// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_ResetStuckAttacking.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTS_ResetStuckAttacking : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTS_ResetStuckAttacking();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	TMap<TWeakObjectPtr<AActor>, float> AttackingDurationMap;

	UPROPERTY(EditAnywhere, Category = "Threshold")
	float MaxAttackingDuration = 1.5f; 
};