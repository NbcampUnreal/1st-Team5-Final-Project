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

	Speed = 600.f;
	TargetRadius = 100.f;
}

EBTNodeResult::Type UBTTask_ClownChase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	
	if (AMarketClownMonster* Clown = Cast<AMarketClownMonster>(Pawn))
	{
		switch (Clown->CurrentMask)
		{
		case ETalMaskType::Yangban:
		case ETalMaskType::Baekjeong:
			TargetRadius = 100.f;
			Speed = 600.f;
			break;
		case ETalMaskType::Bune:
			TargetRadius = 400.f;
			Speed = 500.f;
			break;
		case ETalMaskType::Imae:
			TargetRadius = 400.f;
			Speed = 550.f;
			break;
		default:
			TargetRadius = 200.f;
			break;
		}
	}
	
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

	if (AMarketClownMonster* Clown = Cast<AMarketClownMonster>(AIPawn))
	{
		bool bIsFleeing = false;

		if (Clown->CurrentMask == ETalMaskType::Bune || Clown->CurrentMask == ETalMaskType::Imae)
		{
			const float DesiredMinDistance = 250.f;

			if (Distance < DesiredMinDistance)
			{
				if (UCharacterMovementComponent* Move = Clown->GetCharacterMovement())
				{
					Move->MaxWalkSpeed = 200.f;
				}

				const FVector FleeDir = (AI_Loc - Target_Loc).GetSafeNormal();
				const FVector FleeLocation = AI_Loc + FleeDir * 300.f;

				CachedAICon->MoveToLocation(FleeLocation, 5.f);
				return;
			}
			else
			{
				if (UCharacterMovementComponent* Move = Clown->GetCharacterMovement())
				{
					switch (Clown->CurrentMask)
					{
					case ETalMaskType::Bune:
						Move->MaxWalkSpeed = 400.f;
						break;
					case ETalMaskType::Imae:
						Move->MaxWalkSpeed = 600.f;
						break;
					default:
						break;
					}
				}
			}
		}
	}
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
