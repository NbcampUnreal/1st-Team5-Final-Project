#include "BTTask_PatrolWithIdle_OneEyed.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "Navigation/PathFollowingComponent.h"

UBTTask_PatrolWithIdle_OneEyed::UBTTask_PatrolWithIdle_OneEyed()
{
	NodeName = "Patrol With Idle OneEyed";
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = true;
}

void UBTTask_PatrolWithIdle_OneEyed::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds); 

	FPatrolIdleMemory* Memory = (FPatrolIdleMemory*)NodeMemory;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* AICon = OwnerComp.GetAIOwner();
	APawn* Pawn = AICon ? AICon->GetPawn() : nullptr;

	if (!Memory || !BB || !AICon || !Pawn) return;

	if (BB->GetValueAsBool("bPlayerDetect"))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	if (!Memory->bWaitingForIdle && AICon->GetPathFollowingComponent()->GetStatus() == EPathFollowingStatus::Idle)
	{
		Memory->bWaitingForIdle = true;
		Memory->ElapsedTime = 0.f;
		return;
	}

	if (Memory->bWaitingForIdle)
	{
		Memory->ElapsedTime += DeltaSeconds;

		if (Memory->ElapsedTime >= IdleDuration)
		{
			UE_LOG(LogTemp, Warning, TEXT("[BT OneEyed] Idle Complete → %s"), *GetNameSafe(Pawn));
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}
