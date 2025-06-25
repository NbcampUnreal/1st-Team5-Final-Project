// PayRockGames

#include "BTTask_ClownChase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "PayRock/Enemy/SpecialEnemy/MarketClown/MarketClownMonster.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_ClownChase::UBTTask_ClownChase()
{
	NodeName = "Clown Chase Target";
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_ClownChase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedOwnerComp = &OwnerComp;
	CachedAICon = OwnerComp.GetAIOwner();

	if (!CachedAICon || !CachedAICon->IsValidLowLevelFast()) return EBTNodeResult::Failed;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB || !BB->GetBlackboardAsset()) return EBTNodeResult::Failed;


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
	
	CachedTarget = Target;
	CachedAICon->MoveToActor(Target, 5.f); 

	return EBTNodeResult::InProgress;
}

void UBTTask_ClownChase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!CachedAICon || !CachedTarget || !CachedAICon->GetPawn()) return;

	APawn* AIPawn = CachedAICon->GetPawn();
	const FVector AI_Loc = AIPawn->GetActorLocation();
	const FVector Target_Loc = CachedTarget->GetActorLocation();
	const float Distance = FVector::Dist(AI_Loc, Target_Loc);

	FRotator CurrentRot = AIPawn->GetActorRotation();
	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(AI_Loc, Target_Loc);
	LookAtRot.Pitch = 0.f;
	LookAtRot.Roll = 0.f;
	AIPawn->SetActorRotation(FMath::RInterpTo(CurrentRot, LookAtRot, DeltaSeconds, 5.f));
	
	if (Distance <= TargetRadius)
	{
		CachedAICon->StopMovement();
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
	}
}



void UBTTask_ClownChase::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	if (CachedAICon)
	{
		CachedAICon->StopMovement();
	}

	CachedAICon = nullptr;
	CachedOwnerComp = nullptr;
	CachedTarget = nullptr;
}
