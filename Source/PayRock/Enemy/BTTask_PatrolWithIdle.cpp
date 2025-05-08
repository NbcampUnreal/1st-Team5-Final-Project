#include "BTTask_PatrolWithIdle.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UBTTask_PatrolWithIdle::UBTTask_PatrolWithIdle()
{
    NodeName = "Patrol With Idle";
    bNotifyTick = true;
    bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_PatrolWithIdle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    CachedOwnerComp = &OwnerComp;
    bReachedDestination = false;
    bWaitingForIdle = false;

    AAIController* AICon = OwnerComp.GetAIOwner();
    APawn* Pawn = AICon ? AICon->GetPawn() : nullptr;
    if (!Pawn) return EBTNodeResult::Failed;

    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    if (!BB) return EBTNodeResult::Failed;

    FVector StartPosition = BB->GetValueAsVector("StartPosition");

    FNavLocation RandomLocation;
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    if (NavSys && NavSys->GetRandomReachablePointInRadius(StartPosition, PatrolRadius, RandomLocation))
    {
        AICon->MoveToLocation(RandomLocation.Location, 50.f, true);
        return EBTNodeResult::InProgress;
    }

    return EBTNodeResult::Failed;
}

void UBTTask_PatrolWithIdle::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    if (!BB || !CachedOwnerComp) return;

    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon) return;

    if (BB->GetValueAsBool("bPlayerDetected"))
    {
        GetWorld()->GetTimerManager().ClearTimer(IdleTimerHandle);
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }

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

void UBTTask_PatrolWithIdle::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
    
    GetWorld()->GetTimerManager().ClearTimer(IdleTimerHandle);
    
    bWaitingForIdle = false;
}

void UBTTask_PatrolWithIdle::StartIdleDelay()
{
    if (!CachedOwnerComp) return;

    bWaitingForIdle = true;

    GetWorld()->GetTimerManager().SetTimer(IdleTimerHandle, [this]()
    {
        if (CachedOwnerComp)
        {
            bWaitingForIdle = false;
            FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
        }
    }, IdleDuration, false);
}
