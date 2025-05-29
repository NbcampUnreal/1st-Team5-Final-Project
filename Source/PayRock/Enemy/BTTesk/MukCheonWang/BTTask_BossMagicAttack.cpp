// PayRockGames


#include "BTTask_BossMagicAttack.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"

UBTTask_BossMagicAttack::UBTTask_BossMagicAttack()
{
	NodeName = "Boss Magic Attack";
}

EBTNodeResult::Type UBTTask_BossMagicAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Boss) return EBTNodeResult::Failed;

	UAbilitySystemComponent* ASC = Boss->GetAbilitySystemComponent();
	if (!ASC || !MagicAbility) return EBTNodeResult::Failed;

	ASC->TryActivateAbilityByClass(MagicAbility);

	if (UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent())
	{
		BB->SetValueAsBool(FName("bIsAttacking"), true);
	}

	return EBTNodeResult::Succeeded;
}