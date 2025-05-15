// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_CheckPlayerLooking.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTS_CheckPlayerLooking : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTS_CheckPlayerLooking();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "LookCheck")
	float MaxDetectDistance = 1500.f;

	UPROPERTY(EditAnywhere, Category = "LookCheck")
	float MaxAngleDegrees = 35.f; 
};
