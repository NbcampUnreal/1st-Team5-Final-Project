// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BTTask_PatrolWithIdle.h"
#include "BTTask_PatrolWithIdle_OneEyed.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTTask_PatrolWithIdle_OneEyed : public UBTTask_PatrolWithIdle
{
	GENERATED_BODY()
	
public:
	UBTTask_PatrolWithIdle_OneEyed();
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
