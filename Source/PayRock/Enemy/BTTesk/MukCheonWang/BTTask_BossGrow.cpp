// PayRockGames


#include "BTTask_BossGrow.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"

UBTTask_BossGrow::UBTTask_BossGrow()
{
	NodeName = TEXT("Boss Grow");
	bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_BossGrow::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return EBTNodeResult::Failed;

	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(AICon->GetPawn());
	if (!Boss) return EBTNodeResult::Failed;
	
	Boss->SetActorScale3D(NewScale);

	UCapsuleComponent* Capsule = Boss->GetCapsuleComponent();
	if (Capsule)
	{
		Capsule->SetCapsuleSize(NewCapsuleRadius, NewCapsuleHalfHeight);
	}

	return EBTNodeResult::Succeeded;
}
