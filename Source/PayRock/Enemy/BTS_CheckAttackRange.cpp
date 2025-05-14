// PayRockGames


#include "BTS_CheckAttackRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"

UBTS_CheckAttackRange::UBTS_CheckAttackRange()
{
	NodeName = "Check Attack Range";
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;
}

void UBTS_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AAIController* AICon = OwnerComp.GetAIOwner();
	APawn* AIPawn = AICon ? AICon->GetPawn() : nullptr;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!AIPawn || !BB) return;

	AActor* Target = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
	if (!Target) return;

	AttackRange = BB->GetValueAsFloat("AttackRange");
	DrawDebugSphere(GetWorld(), AIPawn->GetActorLocation(), AttackRange, 12, FColor::Red, false, 1.0f, 0, 2.0f);

	float Distance = FVector::Dist(AIPawn->GetActorLocation(), Target->GetActorLocation());
	BB->SetValueAsBool("bInAttackRange", Distance <= AttackRange);
}
