#include "BTTask_Attack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "PayRock/Enemy/EnemyCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = "Base Attack";
	bNotifyTaskFinished = false; 
	bCreateNodeInstance = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AICon ? AICon->GetPawn() : nullptr);
	if (!Enemy || !AttackAbility) return EBTNodeResult::Failed;
	if (Enemy->IsDead()) return EBTNodeResult::Failed;
	if (UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent())
	{
		if (BB->GetValueAsBool(FName("bIsDead")))
		{
			return EBTNodeResult::Failed;
		}
	}

	if (AICon)
	{
		AICon->StopMovement();
	}

	UAbilitySystemComponent* ASC = Enemy->GetAbilitySystemComponent();
	if (!ASC) return EBTNodeResult::Failed;

	if (ASC->TryActivateAbilityByClass(AttackAbility))
	{
		ASC->TryActivateAbilityByClass(WeaponCollisionAbility);

		if (UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent())
		{
			BB->SetValueAsBool(FName("bIsBusy"), true);
			BB->SetValueAsBool(FName("bInAttackRange"), false);
		}

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
