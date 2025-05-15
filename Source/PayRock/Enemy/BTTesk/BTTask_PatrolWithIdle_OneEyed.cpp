// PayRockGames


#include "BTTask_PatrolWithIdle_OneEyed.h"

#include "AIController.h"


UBTTask_PatrolWithIdle_OneEyed::UBTTask_PatrolWithIdle_OneEyed()
{
	NodeName = "Patrol With Idle OneEyed";
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

void UBTTask_PatrolWithIdle_OneEyed::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB || !CachedOwnerComp) return;

	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return;

	if (!bReachedDestination)
	{
		EPathFollowingStatus::Type Status = AICon->GetPathFollowingComponent()->GetStatus();
		if (Status == EPathFollowingStatus::Idle) 
		{
			bReachedDestination = true;
			StartIdleDelay();
		}
	}
}
