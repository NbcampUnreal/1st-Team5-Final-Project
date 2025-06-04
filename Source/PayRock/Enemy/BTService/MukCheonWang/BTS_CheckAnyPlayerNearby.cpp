// PayRockGames


#include "BTS_CheckAnyPlayerNearby.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"

UBTS_CheckAnyPlayerNearby::UBTS_CheckAnyPlayerNearby()
{
	NodeName = "Check Any Player Near (Using DetectedActors)";
	bNotifyTick = true;
}

void UBTS_CheckAnyPlayerNearby::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AICon = OwnerComp.GetAIOwner();
	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(AICon ? AICon->GetPawn() : nullptr);
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	if (!Boss || !BB) return;

	const FVector BossLocation = Boss->GetActorLocation();
	bool bIsAnyPlayerNear = false;

	for (AActor* Actor : Boss->GetDetectedActors())
	{
		if (!Actor) continue;

		const float Distance = FVector::Dist(BossLocation, Actor->GetActorLocation());
		if (Distance <= NearDistance)
		{
			bIsAnyPlayerNear = true;
			break;
		}
	}

	BB->SetValueAsBool(FName("bIsNearPlayer"), bIsAnyPlayerNear);
}
