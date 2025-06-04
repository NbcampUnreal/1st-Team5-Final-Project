// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PhasePattern.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTTask_PhasePattern : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_PhasePattern();
private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayAbility> Pase_FirstAbility;
	
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayAbility> Pase_SecondAbility;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector PhaseKey;
};
	