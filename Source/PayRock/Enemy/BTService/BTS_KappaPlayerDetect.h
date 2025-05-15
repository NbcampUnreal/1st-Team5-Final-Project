// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_KappaPlayerDetect.generated.h"


UCLASS()
class PAYROCK_API UBTS_KappaPlayerDetect : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTS_KappaPlayerDetect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Kappa|Detection")
	float DetectionRadius = 400.f;
};
