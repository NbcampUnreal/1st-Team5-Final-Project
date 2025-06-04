// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_BossMagicAttack.generated.h"


UCLASS()
class PAYROCK_API UBTTask_BossMagicAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_BossMagicAttack();

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayAbility> MagicAbility;
	
};
