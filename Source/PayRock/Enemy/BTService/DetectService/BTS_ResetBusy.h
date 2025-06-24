// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_ResetBusy.generated.h"

UCLASS()
class PAYROCK_API UBTS_ResetBusy : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTS_ResetBusy();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	TMap<FName, float> BusyElapsedMap;

	UPROPERTY(EditAnywhere, Category = "Reset")
	float BusyResetTime = 2.0f;
};

