#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameplayTagContainer.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PhasePattern.generated.h"

class UGameplayAbility;
class UAbilitySystemComponent;


UCLASS()
class PAYROCK_API UBTTask_PhasePattern : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_PhasePattern();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector PhaseKey;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TSubclassOf<UGameplayAbility> Pase_FirstAbility;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TSubclassOf<UGameplayAbility> Pase_SecondAbility;

private:

	TWeakObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;
	
	TSubclassOf<UGameplayAbility> ThisPhaseAbilityClass;
	
	FDelegateHandle AbilityEndedDelegateHandle;
};
