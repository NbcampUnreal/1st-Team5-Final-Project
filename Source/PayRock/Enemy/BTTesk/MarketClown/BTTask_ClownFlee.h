// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTTask_ClownFlee.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTTask_ClownFlee : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_ClownFlee();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void OnFleeMoveFinished(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	UPROPERTY(EditAnywhere, Category = "Flee")
	float MinFleeDistance;

	UPROPERTY(EditAnywhere, Category = "Flee")
	float MaxFleeDistance;

private:
	UBehaviorTreeComponent* CachedOwnerComp;
	AAIController* CachedAICon;
};