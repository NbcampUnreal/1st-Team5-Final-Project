#include "BTS_ResetTargetActor.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_ResetTargetActor::UBTS_ResetTargetActor()
{
	NodeName = TEXT("No Target Actor");
	bNotifyTick = true;
}

void UBTS_ResetTargetActor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB || !BB->GetBlackboardAsset()) return;

	APawn* AIPawn = AICon->GetPawn();
	if (!AIPawn) return;

	
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
