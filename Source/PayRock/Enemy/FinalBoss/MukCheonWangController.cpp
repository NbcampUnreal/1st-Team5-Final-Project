// PayRockGames


#include "MukCheonWangController.h"


AMukCheonWangController::AMukCheonWangController()
{
	bAttachToPawn = true;
}

void AMukCheonWangController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}