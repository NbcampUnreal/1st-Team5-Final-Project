#include "BTTask_Chase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"

UBTTask_Chase::UBTTask_Chase()
{
	NodeName = "Chase Target";
	bNotifyTaskFinished = true;
	bNotifyTick = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedOwnerComp = &OwnerComp;
	CachedAICon = OwnerComp.GetAIOwner();

	if (!CachedAICon) return EBTNodeResult::Failed;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB || BB->GetValueAsBool("bIsBusy") || BB->GetValueAsBool("bIsAttacking")) return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
	if (!Target) return EBTNodeResult::Failed;

	if (ACharacter* Character = Cast<ACharacter>(CachedAICon->GetPawn()))
	{
		if (UCharacterMovementComponent* Move = Character->GetCharacterMovement())
		{
			Move->MaxWalkSpeed = FMath::Max(Speed, 300.f);
		}
	}

	
	CachedAICon->ReceiveMoveCompleted.RemoveDynamic(this, &UBTTask_Chase::OnMoveCompleted);
	CachedAICon->ReceiveMoveCompleted.AddDynamic(this, &UBTTask_Chase::OnMoveCompleted);

	EPathFollowingRequestResult::Type Result = CachedAICon->MoveToActor(Target, TargetRadius);
	return (Result == EPathFollowingRequestResult::RequestSuccessful) ? EBTNodeResult::InProgress : EBTNodeResult::Failed;
}

void UBTTask_Chase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent())
	{
		if (BB->GetValueAsBool("bIsBeingWatched"))
		{
			if (AAIController* AICon = OwnerComp.GetAIOwner())
			{
				AICon->StopMovement();
			}
			FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
		}
	}
}

void UBTTask_Chase::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (CachedAICon)
	{
		CachedAICon->ReceiveMoveCompleted.RemoveDynamic(this, &UBTTask_Chase::OnMoveCompleted);
	}
	if (CachedOwnerComp)
	{
		const EBTNodeResult::Type FinalResult = (Result == EPathFollowingResult::Success) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
		FinishLatentTask(*CachedOwnerComp, FinalResult);
	}
}

void UBTTask_Chase::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	if (CachedAICon && CachedAICon->ReceiveMoveCompleted.IsAlreadyBound(this, &UBTTask_Chase::OnMoveCompleted))
	{
		CachedAICon->ReceiveMoveCompleted.RemoveDynamic(this, &UBTTask_Chase::OnMoveCompleted);
	}

	CachedAICon = nullptr;
	CachedOwnerComp = nullptr;
}