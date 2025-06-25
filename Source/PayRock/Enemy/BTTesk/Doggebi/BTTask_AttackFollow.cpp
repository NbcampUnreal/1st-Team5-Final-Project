// PayRockGames


#include "BTTask_AttackFollow.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/EnemyCharacter.h"

UBTTask_AttackFollow::UBTTask_AttackFollow()
{
	NodeName = "Attack While Moving";
	bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_AttackFollow::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AICon ? AICon->GetPawn() : nullptr);
	if (!Enemy || !AttackAbility) return EBTNodeResult::Failed;

	UAbilitySystemComponent* ASC = Enemy->GetAbilitySystemComponent();
	if (!ASC) return EBTNodeResult::Failed;

	// 추적 중에도 발동
	if (ASC->TryActivateAbilityByClass(AttackAbility))
	{
		UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
		if (!BB || !BB->GetBlackboardAsset()) return EBTNodeResult::Failed;

		{
			BB->SetValueAsBool("bIsAttacking", true);
		}
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}