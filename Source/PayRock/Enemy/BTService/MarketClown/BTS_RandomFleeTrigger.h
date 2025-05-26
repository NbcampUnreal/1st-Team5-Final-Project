// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_RandomFleeTrigger.generated.h"

UCLASS()
class PAYROCK_API UBTS_RandomFleeTrigger : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTS_RandomFleeTrigger();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "FleeTrigger")
	float TriggerProbability = 0.3f;
};
