// PayRockGames


#include "BTTask_BossAttack.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"

UBTTask_BossAttack::UBTTask_BossAttack()
{
	NodeName = "Booss Attack";
}

EBTNodeResult::Type UBTTask_BossAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Boss) return EBTNodeResult::Failed;

	UAbilitySystemComponent* ASC = Boss->GetAbilitySystemComponent();
	if (!ASC) return EBTNodeResult::Failed;
	
	if (UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent())
	{
		if (BB->GetValueAsBool(FName("bIsDead")))
		{
			return EBTNodeResult::Failed;
		}
	}
	
	if (Boss->CurrentPhase == EBossPhase::Phase1 || Boss->CurrentPhase == EBossPhase::Phase2)
	{
		ASC->TryActivateAbilityByClass(MagicAbility);
	}
	if (Boss->CurrentPhase == EBossPhase::Phase3)
	{
		ASC->TryActivateAbilityByClass(MeleeAbility);
	}

	return EBTNodeResult::Succeeded;
}
