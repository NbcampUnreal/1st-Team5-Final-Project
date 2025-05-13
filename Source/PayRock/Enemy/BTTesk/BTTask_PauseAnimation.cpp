// PayRockGames


#include "BTTask_PauseAnimation.h"

#include "AIController.h"
#include "PayRock/Enemy/SpecialEnemy/SamuraiStatue/SamuraiStatueCharacter.h"


UBTTask_PauseAnimation::UBTTask_PauseAnimation()
{
		NodeName = "Pause Animation";
}

EBTNodeResult::Type UBTTask_PauseAnimation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	ASamuraiStatueCharacter* Statue = Cast<ASamuraiStatueCharacter>(AICon ? AICon->GetPawn() : nullptr);
	if (!Statue) return EBTNodeResult::Failed;

	Statue->PauseAnimation();
	return EBTNodeResult::Succeeded;
	
}
