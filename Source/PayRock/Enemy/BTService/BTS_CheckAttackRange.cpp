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
	if (!BB || !BB->GetBlackboardAsset()) return;


	AActor* Target = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
	if (!Target) return;
	
	const float Distance = FVector::Dist(AIPawn->GetActorLocation(), Target->GetActorLocation());
	BB->SetValueAsBool(FName("bInAttackRange"), Distance <= AttackRange);
}

void UBTS_CheckAttackRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent())
	{
		BB->SetValueAsFloat(FName("AttackRange"), AttackRange);
	}
}