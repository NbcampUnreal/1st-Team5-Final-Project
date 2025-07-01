#include "BTTask_Idle.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"

UBTTask_Idle::UBTTask_Idle()
{
	NodeName = "Idle and Face Target";
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

	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return;

	APawn* AIPawn = AICon->GetPawn();
	if (!AIPawn) return;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (BB)
	{
		AActor* TargetActor = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
		if (TargetActor)
		{
			const FVector Direction = (TargetActor->GetActorLocation() - AIPawn->GetActorLocation()).GetSafeNormal2D();
			if (!Direction.IsNearlyZero())
			{
				const FRotator TargetRotation = Direction.Rotation();
				const FRotator CurrentRotation = AIPawn->GetActorRotation();
				const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, 5.0f); // 5.0f = 회전 속도
				AIPawn->SetActorRotation(NewRotation);
			}
		}
	}

	if (ElapsedTime >= IdleTime)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
