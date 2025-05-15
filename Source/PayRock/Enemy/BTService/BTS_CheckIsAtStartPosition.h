// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_CheckIsAtStartPosition.generated.h"

UCLASS()
class PAYROCK_API UBTS_CheckIsAtStartPosition : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTS_CheckIsAtStartPosition();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Kappa|Detection")
	float AcceptableRadius = 50.0f;
};