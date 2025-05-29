// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_CheckAnyPlayerNearby.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTS_CheckAnyPlayerNearby : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTS_CheckAnyPlayerNearby();

protected:
	UPROPERTY(EditAnywhere, Category = "Range")
	float NearDistance = 300.f;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};