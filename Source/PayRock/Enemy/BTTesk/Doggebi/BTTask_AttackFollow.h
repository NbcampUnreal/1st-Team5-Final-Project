// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_AttackFollow.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTTask_AttackFollow : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	
public:
	UBTTask_AttackFollow();

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayAbility> AttackAbility;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
};
