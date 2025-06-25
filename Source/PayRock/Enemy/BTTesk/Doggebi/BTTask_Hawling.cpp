// PayRockGames


#include "BTTask_Hawling.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/EnemyCharacter.h"

UBTTask_Hawling::UBTTask_Hawling()
{
	
	NodeName = "Hawling";
	bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_Hawling::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB || !BB->GetBlackboardAsset()) return EBTNodeResult::Failed;

	AAIController* AICon = OwnerComp.GetAIOwner();
	
	if (AICon)
	{
		AICon->StopMovement();
	}
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AICon ? AICon->GetPawn() : nullptr);
	if (!Enemy || !HawlingAbility) return EBTNodeResult::Failed;

	UAbilitySystemComponent* ASC = Enemy->GetAbilitySystemComponent();
	if (!ASC) return EBTNodeResult::Failed;

	if (ASC->TryActivateAbilityByClass(HawlingAbility))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("bIsFirstHawling", false);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
