// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossRoar.generated.h"

UCLASS()
class PAYROCK_API UBTTask_BossRoar : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_BossRoar();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Ability")
	TSubclassOf<class UGameplayAbility> RoarAbilityClass;

	UPROPERTY(EditAnywhere, Category = "Ability")
	FGameplayTag ActiveRoarTag;

private:
	bool bIsWaitingForEnd = false;
};
