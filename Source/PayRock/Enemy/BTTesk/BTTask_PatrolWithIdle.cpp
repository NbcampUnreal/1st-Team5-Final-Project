#include "BTTask_PatrolWithIdle.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationPath.h"
#include "DrawDebugHelpers.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"

UBTTask_PatrolWithIdle::UBTTask_PatrolWithIdle()
{
	NodeName = "Patrol With Idle (Obstacle Check)";
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
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	if (!NavSys) return EBTNodeResult::Failed;

	const int32 MaxRetryCount = 5;

	for (int32 Attempt = 0; Attempt < MaxRetryCount; ++Attempt)
	{
		FNavLocation RandomLocation;
		if (!NavSys->GetRandomReachablePointInRadius(StartPosition, PatrolRadius, RandomLocation))
			continue;

		UNavigationPath* Path = NavSys->FindPathToLocationSynchronously(GetWorld(), Pawn->GetActorLocation(), RandomLocation.Location);
		if (!Path || !Path->IsValid() || Path->PathPoints.Num() < 2)
			continue;

		
		bool bBlocked = false;
		for (int32 i = 0; i < Path->PathPoints.Num() - 1; ++i)
		{
			FHitResult Hit;
			if (GetWorld()->LineTraceSingleByChannel(
				Hit,
				Path->PathPoints[i],
				Path->PathPoints[i + 1],
				ECC_Visibility,
				FCollisionQueryParams(FName(TEXT("PatrolObstacleCheck")), false, Pawn)))
			{
				if (AActor* HitActor = Hit.GetActor())
				{
					if (HitActor != Pawn)
					{
						bBlocked = true;
						break;
					}
				}
			}
		}

		if (!bBlocked)
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
