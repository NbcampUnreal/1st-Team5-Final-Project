// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_ResetPlayerDetect.generated.h"


UCLASS()
class PAYROCK_API UBTS_ResetPlayerDetect : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTS_ResetPlayerDetect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "AI")
	float ResetDistance = 1600.0f; 
};