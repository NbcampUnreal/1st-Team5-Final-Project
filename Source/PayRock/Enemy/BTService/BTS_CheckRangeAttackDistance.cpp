// PayRockGames

#include "BTS_CheckRangeAttackDistance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTS_CheckRangeAttackDistance::UBTS_CheckRangeAttackDistance()
{
	NodeName = "Check Range Attack Distance";
	Interval = 0.5f;
	RandomDeviation = 0.f;
}

void UBTS_CheckRangeAttackDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return;

	AActor* TargetActor = Cast<AActor>(BB->GetValueAsObject(TargetActorKey));
	APawn* AIPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;

	if (!AIPawn || !TargetActor)
	{
		BB->SetValueAsBool(InRangeAttackKey, false);
		return;
	}

	const float Distance = FVector::Dist(AIPawn->GetActorLocation(), TargetActor->GetActorLocation());
	const bool bInRange = Distance >= MinDistance && Distance <= MaxDistance;

	BB->SetValueAsBool(InRangeAttackKey, bInRange);
}