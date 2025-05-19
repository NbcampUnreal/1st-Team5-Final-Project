// PayRockGames


#include "BTTask_ClownAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "PayRock/Enemy/EnemyCharacter.h"

UBTTask_ClownAttack::UBTTask_ClownAttack()
{
	NodeName = TEXT("Boss Attack");
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_ClownAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AICon ? AICon->GetPawn() : nullptr);
	if (!Enemy || !BossAttackAbility) return EBTNodeResult::Failed;

	if (AICon)
	{
		AICon->StopMovement();
	}

	UAbilitySystemComponent* ASC = Enemy->GetAbilitySystemComponent();
	if (!ASC) return EBTNodeResult::Failed;

	if (ASC->TryActivateAbilityByClass(BossAttackAbility))
	{
		if (UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent())
		{
			BB->SetValueAsBool("bIsBusy", true);
			BB->SetValueAsBool("bInAttackRange", false);
		}
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
