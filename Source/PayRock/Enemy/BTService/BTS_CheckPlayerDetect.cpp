// PayRockGames
#include "BTS_CheckPlayerDetect.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/EnemyCharacter.h"

UBTS_CheckPlayerDetect::UBTS_CheckPlayerDetect()
{
	NodeName = TEXT("Check Player Detection");
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;
}

void UBTS_CheckPlayerDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* AICon = OwnerComp.GetAIOwner();
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AICon ? AICon->GetPawn() : nullptr);
	
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return;
	if (BB->GetValueAsBool("bPlayerDetect")) return;
	
	bool bSight = BB->GetValueAsBool("bPlayerDetectedBySight");
	bool bLight = BB->GetValueAsBool("bPlayerDetectedByLight");

	bool bFinalDetect = bSight && bLight;

	BB->SetValueAsBool("bPlayerDetect", bFinalDetect);
}
