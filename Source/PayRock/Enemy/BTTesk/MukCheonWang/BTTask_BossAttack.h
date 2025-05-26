// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_BossAttack.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTTask_BossAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_BossAttack();

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayAbility> MagicAbility;
	
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayAbility> MeleeAbility;
};
