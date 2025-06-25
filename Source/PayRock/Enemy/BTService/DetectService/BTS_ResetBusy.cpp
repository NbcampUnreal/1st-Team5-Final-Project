// PayRockGames


#include "BTS_ResetBusy.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_ResetBusy::UBTS_ResetBusy()
{
	NodeName = TEXT("Reset Busy");
	bNotifyTick = true;
}

void UBTS_ResetBusy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB || !BB->GetBlackboardAsset()) return;


	APawn* AIPawn = AICon->GetPawn();
	if (!AIPawn) return;
	
	if (BB->GetValueAsBool("bIsBusy"))
	{
		FName AIKey = AICon->GetFName();
		float& ElapsedTime = BusyElapsedMap.FindOrAdd(AIKey);
		ElapsedTime += DeltaSeconds;

		if (ElapsedTime > BusyResetTime)
		{
			BB->SetValueAsBool("bIsBusy", false);
			BusyElapsedMap.Remove(AIKey);
		}
	}
	else
	{
		BusyElapsedMap.Remove(AICon->GetFName());
	}
}