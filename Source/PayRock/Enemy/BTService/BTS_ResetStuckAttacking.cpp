// PayRockGames


#include "BTS_ResetStuckAttacking.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"

UBTS_ResetStuckAttacking::UBTS_ResetStuckAttacking()
{
	NodeName = "Reset bIsAttacking if stuck";
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;
}

void UBTS_ResetStuckAttacking::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return;

	const bool bIsAttacking = BB->GetValueAsBool(FName("bIsAttacking"));
	AActor* OwnerActor = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (!OwnerActor) return;

	if (bIsAttacking)
	{
		float& Elapsed = AttackingDurationMap.FindOrAdd(OwnerActor);
		Elapsed += DeltaSeconds;

		if (Elapsed >= MaxAttackingDuration)
		{
			BB->SetValueAsBool(FName("bIsAttacking"), false);
			Elapsed = 0.f;
		}
	}
	else
	{
		AttackingDurationMap.FindOrAdd(OwnerActor) = 0.f;
	}
}
