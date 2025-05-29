// PayRockGames


#include "BTTask_Idle.h"
#include "AIController.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"

UBTTask_Idle::UBTTask_Idle()
{
	NodeName = "Idle";
	bNotifyTick = false;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_Idle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UWorld* World = OwnerComp.GetWorld();
	if (!World) return EBTNodeResult::Failed;
	
	FTimerDelegate TimerDel;
	TimerDel.BindUObject(this, &UBTTask_Idle::FinishIdle, &OwnerComp);

	World->GetTimerManager().SetTimer(IdleTimerHandle, TimerDel, IdleTime, false);

	return EBTNodeResult::InProgress;
}

void UBTTask_Idle::FinishIdle(UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
