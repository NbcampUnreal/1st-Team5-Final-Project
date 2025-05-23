// PayRockGames

#include "BTS_Reset.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_Reset::UBTS_Reset()
{
	NodeName = TEXT("Initialize Blackboard");
	bNotifyTick = true;
}

void UBTS_Reset::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon)
	{
		return;
	}

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB)
	{
		return;
	}

	APawn* AIPawn = AICon->GetPawn();
	if (!AIPawn)
	{
		return;
	}

	if (BB->GetValueAsVector("StartPosition").IsNearlyZero())
	{
		BB->SetValueAsVector("StartPosition", AIPawn->GetActorLocation());
	}
	
	AActor* Target = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
	if (!IsValid(Target))
	{
		BB->ClearValue("TargetActor");
		BB->SetValueAsBool("bDetect", false);
		BB->SetValueAsBool("bIsBusy", false);
		BB->SetValueAsBool("bPlayerDetected", false);
		BB->SetValueAsBool("bInAttackRange", false);
	}
}
