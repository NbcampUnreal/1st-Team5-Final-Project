// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_BossMeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTTask_BossMeleeAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_BossMeleeAttack();

	UPROPERTY(EditAnywhere, Category = "Ability")
	TSubclassOf<class UGameplayAbility> MeleeAbility;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};