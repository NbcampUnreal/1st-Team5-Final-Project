// PayRockGames


#include "BTTask_ChargeAttack.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/EnemyCharacter.h"


UBTTask_ChargeAttack::UBTTask_ChargeAttack()
{
	NodeName = "Charge Attack";
}

EBTNodeResult::Type UBTTask_ChargeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	AAIController* AICon = OwnerComp.GetAIOwner();
	
	if (AICon)
	{
		AICon->StopMovement();
	}
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AICon ? AICon->GetPawn() : nullptr);
	if (!Enemy || !ChargeAttackAbility) return EBTNodeResult::Failed;

	UAbilitySystemComponent* ASC = Enemy->GetAbilitySystemComponent();
	if (!ASC) return EBTNodeResult::Failed;

	if (ASC->TryActivateAbilityByClass(ChargeAttackAbility))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("bIsAttacking", true);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
