// PayRockGames


#include "BTTask_Attack.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "PayRock/Enemy/EnemyCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = "Base Attack";
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AICon ? AICon->GetPawn() : nullptr);
	if (!Enemy || !AttackAbility) return EBTNodeResult::Failed;
	
	if (AICon)
	{
		AICon->StopMovement();
	}
	UAbilitySystemComponent* ASC = Enemy->GetAbilitySystemComponent();
	if (!ASC) return EBTNodeResult::Failed;

	if (ASC->TryActivateAbilityByClass(AttackAbility))
	{
		if (UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent())
		{
			BB->SetValueAsBool(FName("bIsBusy"), true);
			BB->SetValueAsBool("bInAttackRange", false);
		}
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

