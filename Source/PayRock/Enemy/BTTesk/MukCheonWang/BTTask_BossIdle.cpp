// PayRockGames


#include "BTTask_BossIdle.h"
#include "AIController.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"

UBTTask_BossIdle::UBTTask_BossIdle()
{
	NodeName = "Boss Idle";
	bNotifyTick = false;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_BossIdle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UWorld* World = OwnerComp.GetWorld();
	if (!World) return EBTNodeResult::Failed;
	
	FTimerDelegate TimerDel;
	TimerDel.BindUObject(this, &UBTTask_BossIdle::FinishIdle, &OwnerComp);

	World->GetTimerManager().SetTimer(IdleTimerHandle, TimerDel, IdleTime, false);

	return EBTNodeResult::InProgress;
}

void UBTTask_BossIdle::FinishIdle(UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
