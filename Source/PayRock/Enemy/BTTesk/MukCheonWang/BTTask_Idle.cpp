// PayRockGames


#include "BTTask_Idle.h"
#include "AIController.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"

UBTTask_Idle::UBTTask_Idle()
{
	NodeName = "Idle";
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_Idle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ElapsedTime = 0.f;
	return EBTNodeResult::InProgress;
}

void UBTTask_Idle::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ElapsedTime += DeltaSeconds;

	if (ElapsedTime >= IdleTime)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
