
#include "BTTask_ActivateDetectAbility.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/EnemyCharacter.h"

UBTTask_ActivateDetectAbility::UBTTask_ActivateDetectAbility()
{
	NodeName = "Kappa Activate Detect Ability";
}

EBTNodeResult::Type UBTTask_ActivateDetectAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AICon ? AICon->GetPawn() : nullptr);
	if (!Enemy || !DetectAbility) return EBTNodeResult::Failed;

	UAbilitySystemComponent* ASC = Enemy->GetAbilitySystemComponent();
	if (!ASC) return EBTNodeResult::Failed;

	if (ASC->TryActivateAbilityByClass(DetectAbility))
	{
		if (UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent())
		{
			BB->SetValueAsBool(DetectKey, true);
		}
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
