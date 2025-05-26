// PayRockGames


#include "BTTask_BossChase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"


UBTTask_BossChase::UBTTask_BossChase()
{
	NodeName = "Base Attack";
}

EBTNodeResult::Type UBTTask_BossChase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(AICon->GetPawn());
	if (!Boss || Boss->CurrentPhase != EBossPhase::Phase3)
	{
		return EBTNodeResult::Failed;
	}

	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	if (!Target) return EBTNodeResult::Failed;

	AICon->MoveToActor(Target, 100.f, true);
	
	return EBTNodeResult::Succeeded;
}
