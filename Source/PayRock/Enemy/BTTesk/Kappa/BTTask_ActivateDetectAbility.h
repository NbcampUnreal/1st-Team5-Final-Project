// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ActivateDetectAbility.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTTask_ActivateDetectAbility : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_ActivateDetectAbility();

protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayAbility> DetectAbility;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FName DetectKey = "bDetect";

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};