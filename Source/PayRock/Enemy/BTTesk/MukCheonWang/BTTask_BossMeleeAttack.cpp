// PayRockGames


#include "BTTask_BossMeleeAttack.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"

UBTTask_BossMeleeAttack::UBTTask_BossMeleeAttack()
{
	NodeName = "Boss Melee Attack";
}

EBTNodeResult::Type UBTTask_BossMeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Boss) return EBTNodeResult::Failed;

	UAbilitySystemComponent* ASC = Boss->GetAbilitySystemComponent();
	if (!ASC || !MeleeAbility) return EBTNodeResult::Failed;

	ASC->TryActivateAbilityByClass(MeleeAbility);

	if (UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent())
	{
		BB->SetValueAsBool(FName("bIsAttacking"), true);
	}

	return EBTNodeResult::Succeeded;
}
