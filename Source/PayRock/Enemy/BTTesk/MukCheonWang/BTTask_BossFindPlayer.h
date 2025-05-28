// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_BossFindPlayer.generated.h"


UCLASS()
class PAYROCK_API UBTTask_BossFindPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_BossFindPlayer();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category="AI")
	float MontagePlayRate = 1.0f;

	UPROPERTY(EditAnywhere, Category="AI")
	float WaitTime = 2.0f;
};