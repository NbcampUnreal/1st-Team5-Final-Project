#include "BTS_ResetPlayerDetect.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PayRock/Character/PRCharacter.h"
#include "Kismet/GameplayStatics.h"

UBTS_ResetPlayerDetect::UBTS_ResetPlayerDetect()
{
	NodeName = TEXT("Reset Player Detect if Far from StartPosition");
	bNotifyTick = true;
}

void UBTS_ResetPlayerDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB || !BB->GetBlackboardAsset()) return;


	APawn* Controlled = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (!Controlled) return;

	FVector StartPos = BB->GetValueAsVector(FName("StartPosition"));
	float DistanceFromStart = FVector::Dist(Controlled->GetActorLocation(), StartPos);

	if (DistanceFromStart > ResetDistance)
	{
		BB->ClearValue(FName("TargetActor"));
		BB->SetValueAsBool(FName("bPlayerDetect"), false);
		BB->SetValueAsBool(FName("bInAttackRange"), false);
	}
}
