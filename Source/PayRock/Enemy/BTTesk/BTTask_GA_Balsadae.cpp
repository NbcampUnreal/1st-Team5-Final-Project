// PayRockGames


#include "BTTask_GA_Balsadae.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/EnemyCharacter.h"

UBTTask_GA_Balsadae::UBTTask_GA_Balsadae()
{
	NodeName = "GA_Balsadae";
}

EBTNodeResult::Type UBTTask_GA_Balsadae::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AICon ? AICon->GetPawn() : nullptr);
	if (!Enemy || !EveryAbility) return EBTNodeResult::Failed;
	
	if (AICon)
	{
		AICon->StopMovement();
	}
	UAbilitySystemComponent* ASC = Enemy->GetAbilitySystemComponent();
	if (!ASC) return EBTNodeResult::Failed;

	if (ASC->TryActivateAbilityByClass(EveryAbility))
	{
		UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
		if (!BB || !BB->GetBlackboardAsset()) return EBTNodeResult::Failed;

		{
			//BB 값 바꾸는곳
			//BB->SetValueAsBool("bIsNeedMaskChange", false);
			
			BB->SetValueAsBool("bIsNeedMaskChange", false);
			
		}
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
