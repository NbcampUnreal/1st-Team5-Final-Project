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
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedOwnerComp = &OwnerComp;
	CachedAICon = OwnerComp.GetAIOwner();

	if (!CachedAICon || !CachedAICon->IsValidLowLevelFast()) return EBTNodeResult::Failed;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;

	APawn* Pawn = CachedAICon->GetPawn();
	if (!Pawn || BB->GetValueAsBool("bIsBusy") || BB->GetValueAsBool("bIsAttacking"))
		return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
	if (!Target || !Target->IsValidLowLevelFast()) return EBTNodeResult::Failed;

	if (ACharacter* Character = Cast<ACharacter>(Pawn))
	{
		if (UCharacterMovementComponent* Move = Character->GetCharacterMovement())
		{
			Move->MaxWalkSpeed = Speed;
		}
	}
	
	EPathFollowingRequestResult::Type Result = CachedAICon->MoveToActor(Target, TargetRadius);
	if (Result == EPathFollowingRequestResult::RequestSuccessful)
	{
		if (!CachedAICon->ReceiveMoveCompleted.IsAlreadyBound(this, &UBTTask_Chase::OnMoveCompleted))
		{
			CachedAICon->ReceiveMoveCompleted.AddDynamic(this, &UBTTask_Chase::OnMoveCompleted);
		}
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UBTTask_Chase::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (!CachedAICon || !CachedOwnerComp)
		return;

	if (CachedAICon->ReceiveMoveCompleted.IsAlreadyBound(this, &UBTTask_Chase::OnMoveCompleted))
	{
		CachedAICon->ReceiveMoveCompleted.RemoveDynamic(this, &UBTTask_Chase::OnMoveCompleted);
	}

	const EBTNodeResult::Type FinalResult =
		(Result == EPathFollowingResult::Success) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;

	FinishLatentTask(*CachedOwnerComp, FinalResult);
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
