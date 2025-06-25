
#include "BTS_CheckIsAtStartPosition.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTS_CheckIsAtStartPosition::UBTS_CheckIsAtStartPosition()
{
	NodeName = TEXT("Check Is At Start Position");
	bNotifyTick = true;
}

void UBTS_CheckIsAtStartPosition::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB || !BB->GetBlackboardAsset()) return;

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (!BB || !ControlledPawn) return;

	FVector CurrentLocation = ControlledPawn->GetActorLocation();
	FVector StartLocation = BB->GetValueAsVector(TEXT("StartPosition"));

	bool bIsAtStart = FVector::Dist(CurrentLocation, StartLocation) <= AcceptableRadius;
	BB->SetValueAsBool(TEXT("bIsAtStartPosition"), bIsAtStart);
	BB->SetValueAsBool(TEXT("bDetect"), false);
}
