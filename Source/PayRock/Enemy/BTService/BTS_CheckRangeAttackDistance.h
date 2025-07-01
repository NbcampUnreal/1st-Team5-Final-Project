// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_CheckRangeAttackDistance.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTS_CheckRangeAttackDistance : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTS_CheckRangeAttackDistance();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Distance")
	float MinDistance = 300.f;

	UPROPERTY(EditAnywhere, Category = "Distance")
	float MaxDistance = 1200.f;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FName InRangeAttackKey = "bInRangeAttackRange";

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FName TargetActorKey = "TargetActor";
};