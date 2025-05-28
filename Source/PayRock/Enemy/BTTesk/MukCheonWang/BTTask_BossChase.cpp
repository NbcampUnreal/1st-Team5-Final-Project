// PayRockGames


#include "BTTask_BossChase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"


UBTTask_BossChase::UBTTask_BossChase()
{
	NodeName = "Boss Chase";
}

EBTNodeResult::Type UBTTask_BossChase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(AICon ? AICon->GetPawn() : nullptr);
	if (!Boss) return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	if (!Target) return EBTNodeResult::Failed;
	
	if (Boss->CurrentPhase == EBossPhase::Phase3)
	{
		AICon->MoveToActor(Target, 100.f, true);
	}
	
	return EBTNodeResult::Succeeded;
}
