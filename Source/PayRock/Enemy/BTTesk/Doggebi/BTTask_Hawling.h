// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Hawling.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTTask_Hawling : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	
public:
	UBTTask_Hawling();

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayAbility> HawlingAbility;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
	
};
