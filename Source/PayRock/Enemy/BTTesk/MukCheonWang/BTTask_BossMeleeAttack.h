#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameplayAbilitySpec.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_BossMeleeAttack.generated.h"

class UGameplayAbility;


UCLASS()
class PAYROCK_API UBTTask_BossMeleeAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_BossMeleeAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Ability")
	TSubclassOf<UGameplayAbility> MeleeAbility;

	TSubclassOf<UGameplayAbility> ThisAbilityClass;
	TWeakObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;
	TWeakObjectPtr<UAbilitySystemComponent> CachedASC;
	FDelegateHandle AbilityEndedHandle;


};
