#include "BTTask_PatrolWithIdle.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationPath.h"
#include "DrawDebugHelpers.h"
#include "Navigation/PathFollowingComponent.h"

UBTTask_PatrolWithIdle::UBTTask_PatrolWithIdle()
{
	NodeName = "Patrol With Idle (Tick Accumulation)";
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_PatrolWithIdle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FPatrolIdleMemory* Memory = (FPatrolIdleMemory*)NodeMemory;
	Memory->ElapsedTime = 0.f;
	Memory->bWaitingForIdle = false;

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
		if (ACharacter* Character = Cast<ACharacter>(Pawn))
		{
			if (UCharacterMovementComponent* Move = Character->GetCharacterMovement())
			{
				Move->MaxWalkSpeed = PatrolMoveSpeed;
			}
		}
		AICon->MoveToLocation(RandomLocation.Location, 50.f, true);
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UBTTask_PatrolWithIdle::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FPatrolIdleMemory* Memory = (FPatrolIdleMemory*)NodeMemory;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* AICon = OwnerComp.GetAIOwner();
	APawn* Pawn = AICon ? AICon->GetPawn() : nullptr;

	if (!BB || !AICon || !Pawn) return;

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
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}

uint16 UBTTask_PatrolWithIdle::GetInstanceMemorySize() const
{
	return sizeof(FPatrolIdleMemory);
}
