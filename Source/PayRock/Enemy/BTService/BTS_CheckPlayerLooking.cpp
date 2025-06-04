// PayRockGames


#include "BTS_CheckPlayerLooking.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "PayRock/Character/PRCharacter.h"

UBTS_CheckPlayerLooking::UBTS_CheckPlayerLooking()
{
	NodeName = "Check If Player Is Looking";
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;
}

void UBTS_CheckPlayerLooking::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AICon = OwnerComp.GetAIOwner();
	APawn* SelfPawn = AICon ? AICon->GetPawn() : nullptr;
	if (!SelfPawn) return;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return;

	TArray<AActor*> PlayerActors;
	UGameplayStatics::GetAllActorsOfClass(SelfPawn->GetWorld(), APRCharacter::StaticClass(), PlayerActors);

	for (AActor* PlayerActor : PlayerActors)
	{
		if (PlayerActor == SelfPawn) continue;

		float Distance = FVector::Dist(SelfPawn->GetActorLocation(), PlayerActor->GetActorLocation());
		if (Distance > MaxDetectDistance) continue;

		FVector ToStatue = (SelfPawn->GetActorLocation() - PlayerActor->GetActorLocation()).GetSafeNormal();
		FVector PlayerForward = PlayerActor->GetActorForwardVector();
		float Dot = FVector::DotProduct(PlayerForward, ToStatue);
		float Angle = FMath::RadiansToDegrees(FMath::Acos(Dot));

		if (Angle < MaxAngleDegrees)
		{
			BB->SetValueAsBool("bIsBeingWatched", true);
			return;
		}
		else
		{
			BB->SetValueAsBool("bIsBeingWatched", false);
		}
	}

	if (PlayerActors.Num() == 0)
	{
		BB->SetValueAsBool("bIsBeingWatched", false);
	}
}
