// PayRockGames


#include "BTS_BossMeleeAttackRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"

UBTS_BossMeleeAttackRange::UBTS_BossMeleeAttackRange()
{
	NodeName = "Check Melee Attack Range";
	bNotifyTick = true;
}

void UBTS_BossMeleeAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AICon = OwnerComp.GetAIOwner();
	APawn* AIPawn = AICon ? AICon->GetPawn() : nullptr;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	if (!AIPawn || !BB) return;

	AActor* Target = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
	if (!Target) return;

	float Distance = FVector::Dist(AIPawn->GetActorLocation(), Target->GetActorLocation());
	bool bInRange = Distance <= MeleeRange;

	BB->SetValueAsBool(FName("bInMeleeRange"), bInRange);
}
