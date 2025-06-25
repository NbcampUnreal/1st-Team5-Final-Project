// PayRockGames


#include "BTS_UpdateClosestTarget.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "GameFramework/Actor.h"
#include "PayRock/Enemy/EnemyController.h"

UBTS_UpdateClosestTarget::UBTS_UpdateClosestTarget()
{
	NodeName = TEXT("Update Closest Target");
}

void UBTS_UpdateClosestTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemyController* AICon = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	if (!AICon) return;

	APawn* AIPawn = AICon->GetPawn();
	if (!AIPawn) return;

	const TArray<AActor*>& Candidates = AICon->GetSensedActors();
	float ClosestDistSq = FLT_MAX;
	AActor* ClosestActor = nullptr;

	for (AActor* Target : Candidates)
	{
		if (!IsValid(Target) || Target->IsActorBeingDestroyed()) continue;

		const float DistSq = FVector::DistSquared(AIPawn->GetActorLocation(), Target->GetActorLocation());
		if (DistSq < ClosestDistSq)
		{
			ClosestDistSq = DistSq;
			ClosestActor = Target;
		}
	}

	if (ClosestActor)
	{
		UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
		if (!BB || !BB->GetBlackboardAsset()) return;

		if (BB)
		{
			BB->SetValueAsObject(GetSelectedBlackboardKey(), ClosestActor);
			BB->SetValueAsBool(TEXT("bPlayerDetect"), true);
		}
	}
}
