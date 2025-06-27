// PayRockGames


#include "BTTask_NormalAttack.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/EnemyCharacter.h"

UBTTask_NormalAttack::UBTTask_NormalAttack()
{
	NodeName = "Normal Attack";
}

EBTNodeResult::Type UBTTask_NormalAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AICon ? AICon->GetPawn() : nullptr);
	if (!Enemy || !AttackAbility) return EBTNodeResult::Failed;
	
	UAbilitySystemComponent* ASC = Enemy->GetAbilitySystemComponent();
	
	if (!ASC) return EBTNodeResult::Failed;

	
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	bool bblackIsAttacking = BB->GetValueAsBool("bIsAttacking");
	UE_LOG(LogTemp, Warning, TEXT("[UBTTask_NormalAttack] before %s "),bblackIsAttacking ? TEXT("true") : TEXT("false"));
	
	if (ASC->TryActivateAbilityByClass(AttackAbility))
	{
		
		UE_LOG(LogTemp, Warning, TEXT("[UBTTask_NormalAttack] After1 %s "),bblackIsAttacking ? TEXT("true") : TEXT("false"));
		if (!BB || !BB->GetBlackboardAsset()) return EBTNodeResult::Failed;

		
		BB->SetValueAsBool("bInAttackRange", false);
		BB->SetValueAsBool("bIsAttacking", true);

		
		UE_LOG(LogTemp, Warning, TEXT("[UBTTask_NormalAttack] After2 %s "),bblackIsAttacking ? TEXT("true") : TEXT("false"));

		return EBTNodeResult::Succeeded;
	}

	UE_LOG(LogTemp, Warning, TEXT("[UBTTask_NormalAttack] After3 %s "),bblackIsAttacking ? TEXT("true") : TEXT("false"));
	return EBTNodeResult::Failed;
}
