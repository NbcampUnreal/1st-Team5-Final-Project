// PayRockGames


#include "BTS_BossMagicAttackRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"

UBTS_BossMagicAttackRange::UBTS_BossMagicAttackRange()
{
	NodeName = "Check Magic Attack Range";
	bNotifyTick = true;
}

void UBTS_BossMagicAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* AICon = OwnerComp.GetAIOwner();
	APawn* AIPawn = AICon ? AICon->GetPawn() : nullptr;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB || !BB->GetBlackboardAsset()) return;


	if (!AIPawn)
	{
		return;
	}

	if (!BB)
	{
		return;
	}

	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(AIPawn);
	if (!Boss)
	{
		return;
	}
	
	AActor* Target = Cast<AActor>(BB->GetValueAsObject(FName("TargetActor")));
	if (!Target)
	{
		return;
	}

	const float Distance = FVector::Dist(AIPawn->GetActorLocation(), Target->GetActorLocation());
	const bool bInRange = Distance <= MagicRange;
	
	BB->SetValueAsBool(FName("bInMagicRange"), bInRange);
}
