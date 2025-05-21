// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ChargeAttack.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTTask_ChargeAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	UBTTask_ChargeAttack();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UGameplayAbility> ChargeAttackAbility;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
