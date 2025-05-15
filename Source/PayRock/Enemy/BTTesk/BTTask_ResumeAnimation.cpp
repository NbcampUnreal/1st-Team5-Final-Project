// PayRockGames


#include "BTTask_ResumeAnimation.h"

#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PayRock/Enemy/SpecialEnemy/SamuraiStatue/SamuraiStatueCharacter.h"


UBTTask_ResumeAnimation::UBTTask_ResumeAnimation()
{
	NodeName = "Resume Animation";
}

EBTNodeResult::Type UBTTask_ResumeAnimation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	ASamuraiStatueCharacter* Statue = Cast<ASamuraiStatueCharacter>(AICon ? AICon->GetPawn() : nullptr);

	if (!Statue) return EBTNodeResult::Failed;

	Statue->ResumeAnimation();
	
	if (UCharacterMovementComponent* MoveComp = Statue->GetCharacterMovement())
	{
		MoveComp->SetMovementMode(MOVE_Walking);
	}
	return EBTNodeResult::Succeeded;
}
