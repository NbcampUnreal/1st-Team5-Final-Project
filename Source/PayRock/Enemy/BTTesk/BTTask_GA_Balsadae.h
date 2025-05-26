// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GA_Balsadae.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTTask_GA_Balsadae : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_GA_Balsadae();

protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayAbility> EveryAbility;
	

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
