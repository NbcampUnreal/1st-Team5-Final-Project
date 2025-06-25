// PayRockGames


#include "BTS_RandomFleeTrigger.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTS_RandomFleeTrigger::UBTS_RandomFleeTrigger()
{
	NodeName = TEXT("Random Flee Trigger");
	Interval = 1.5f; 
	RandomDeviation = 0.5f;
}

void UBTS_RandomFleeTrigger::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB || !BB->GetBlackboardAsset()) return;


	const float RandomValue = FMath::FRand();
	if (RandomValue <= TriggerProbability)
	{
		BB->SetValueAsBool(FName("bIsFlee"), true);
	}
}
