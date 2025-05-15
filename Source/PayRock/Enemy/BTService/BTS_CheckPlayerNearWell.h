// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_CheckPlayerNearWell.generated.h"

UCLASS()
class PAYROCK_API UBTS_CheckPlayerNearWell : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTS_CheckPlayerNearWell();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Kappa|Detection")
	float DetectionRadius = 300.f;
};

