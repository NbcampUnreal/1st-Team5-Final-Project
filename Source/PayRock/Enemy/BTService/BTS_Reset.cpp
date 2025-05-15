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

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return;
	
	if (BB->GetValueAsObject(FName("TargetActor")))
	{
		return;
	}

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!AIPawn) return;
	
	if (BB->GetValueAsVector(FName("StartPosition")).IsNearlyZero())
	{
		BB->SetValueAsVector(FName("StartPosition"), AIPawn->GetActorLocation());
	}

	BB->SetValueAsBool(FName("bDetect"), false);
	BB->SetValueAsBool(FName("bIsBusy"), false);
	BB->SetValueAsBool(FName("bPlayerDetected"), false);
	BB->SetValueAsBool(FName("bInAttackRange"), false);
}
