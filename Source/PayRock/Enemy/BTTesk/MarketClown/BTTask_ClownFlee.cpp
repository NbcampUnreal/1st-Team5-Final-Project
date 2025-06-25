#include "BTTask_ClownFlee.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Navigation/PathFollowingComponent.h"
#include "PayRock/Enemy/SpecialEnemy/MarketClown/MarketClownMonster.h"

UBTTask_ClownFlee::UBTTask_ClownFlee()
{
	NodeName = TEXT("Flee From Target");
	bNotifyTaskFinished = true;
	bCreateNodeInstance = true;
	MinFleeDistance = 300.f;
	MaxFleeDistance = 500.f;
}

EBTNodeResult::Type UBTTask_ClownFlee::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedAICon = OwnerComp.GetAIOwner();
	CachedOwnerComp = &OwnerComp;

	if (!CachedAICon) return EBTNodeResult::Failed;

	APawn* AIPawn = CachedAICon->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB || !BB->GetBlackboardAsset()) return EBTNodeResult::Failed;


	BB->SetValueAsBool("bIsBusy", false);
	BB->SetValueAsBool("bIsAttacking", true);
	BB->SetValueAsBool("bInAttackRange", false);

	AActor* TargetActor = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
	if (!TargetActor) return EBTNodeResult::Failed;

	const FVector MyLocation = AIPawn->GetActorLocation();
	const FVector TargetLocation = TargetActor->GetActorLocation();
	FVector FleeDirection = (MyLocation - TargetLocation).GetSafeNormal();
	float RandomDistance = FMath::FRandRange(MinFleeDistance, MaxFleeDistance);
	FVector RawFleeLocation = MyLocation + FleeDirection * RandomDistance;

	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);
	LookAtRot.Pitch = 0.f;
	LookAtRot.Roll = 0.f;
	AIPawn->SetActorRotation(LookAtRot);

	if (ACharacter* Character = Cast<ACharacter>(AIPawn))
	{
		if (UCharacterMovementComponent* Move = Character->GetCharacterMovement())
		{
			Move->bUseControllerDesiredRotation = false;
			Character->bUseControllerRotationYaw = false;
		}
	}

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(AIPawn->GetWorld());
	if (NavSys)
	{
		FNavLocation NavLocation;
		if (NavSys->GetRandomPointInNavigableRadius(RawFleeLocation, 100.f, NavLocation))
		{
			FAIMoveRequest MoveRequest;
			MoveRequest.SetGoalLocation(NavLocation.Location);
			MoveRequest.SetAcceptanceRadius(5.f);

			FNavPathSharedPtr NavPath;
			CachedAICon->MoveTo(MoveRequest, &NavPath);
			
			CachedAICon->ReceiveMoveCompleted.AddDynamic(this, &UBTTask_ClownFlee::OnFleeMoveFinished);

			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::Failed;
}

void UBTTask_ClownFlee::OnFleeMoveFinished(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (!CachedAICon || !CachedOwnerComp) return;

	if (UBlackboardComponent* BB = CachedAICon->GetBlackboardComponent())
	{
		BB->SetValueAsBool("bIsFlee", false);
		BB->SetValueAsBool("bIsAttacking", false);
	}

	CachedAICon->ReceiveMoveCompleted.RemoveDynamic(this, &UBTTask_ClownFlee::OnFleeMoveFinished);
	FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
}