#include "BTTask_PlayDetectMontage.h"
#include "AIController.h"
#include "PayRock/Enemy/EnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_PlayDetectMontage::UBTTask_PlayDetectMontage()
{
	NodeName = "Activate Detect Ability";
	bNotifyTaskFinished = true;
	bCreateNodeInstance = true; 
}

EBTNodeResult::Type UBTTask_PlayDetectMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AIController->GetPawn());
	if (!Enemy) return EBTNodeResult::Failed;

	UAbilitySystemComponent* ASC = Enemy->GetAbilitySystemComponent();
	if (!ASC) return EBTNodeResult::Failed;
	if (!ASC->TryActivateAbilityByClass(DetectAbility))
	{
		return EBTNodeResult::Failed;
	}

	if (UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent())
	{
		BB->SetValueAsBool(FName("bDetect"), true);
	}

	return EBTNodeResult::Succeeded;
}
