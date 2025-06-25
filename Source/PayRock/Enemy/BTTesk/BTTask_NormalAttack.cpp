// PayRockGames


#include "BTTask_NormalAttack.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/EnemyCharacter.h"

UBTTask_NormalAttack::UBTTask_NormalAttack()
{
	NodeName = "Normal Attack";
}

EBTNodeResult::Type UBTTask_NormalAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AICon ? AICon->GetPawn() : nullptr);
	if (!Enemy || !AttackAbility) return EBTNodeResult::Failed;
	
	UAbilitySystemComponent* ASC = Enemy->GetAbilitySystemComponent();
	if (!ASC) return EBTNodeResult::Failed;

	if (ASC->TryActivateAbilityByClass(AttackAbility))
	{
		UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
		if (!BB || !BB->GetBlackboardAsset()) return EBTNodeResult::Failed;

		{
			BB->SetValueAsBool("bInAttackRange", false);
			BB->SetValueAsBool("bIsAttacking", true);
		}
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
