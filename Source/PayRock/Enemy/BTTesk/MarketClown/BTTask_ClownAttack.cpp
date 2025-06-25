#include "BTTask_ClownAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "PayRock/Enemy/EnemyCharacter.h"
#include "PayRock/Enemy/SpecialEnemy/MarketClown/MarketClownMonster.h"

UBTTask_ClownAttack::UBTTask_ClownAttack()
{
	NodeName = TEXT("Boss Attack");
	bNotifyTaskFinished = false;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_ClownAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	AMarketClownMonster* Clown = Cast<AMarketClownMonster>(AICon ? AICon->GetPawn() : nullptr);
	if (!Clown || !AttackAbility) return EBTNodeResult::Failed;
	if (Clown->IsDead()) return EBTNodeResult::Failed;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB || !BB->GetBlackboardAsset()) return EBTNodeResult::Failed;

	{
		if (BB->GetValueAsBool(FName("bIsDead")))
		{
			return EBTNodeResult::Failed;
		}
	}

	if (AICon)
	{
		AICon->StopMovement();
	}

	UAbilitySystemComponent* ASC = Clown->GetAbilitySystemComponent();

	switch (Clown->CurrentMask)
	{
	case ETalMaskType::Yangban:
	case ETalMaskType::Baekjeong:
		if (ASC->TryActivateAbilityByClass(AttackAbility))
		{
			if (BB)
			{
				BB->SetValueAsBool("bIsBusy", true);
				BB->SetValueAsBool("bInAttackRange", false);
			}
			return EBTNodeResult::Succeeded;
		}
		break;

	case ETalMaskType::Bune:
		if (ASC->TryActivateAbilityByClass(BuneSpecialAttackAbility))
		{
			if (BB)
			{
				BB->SetValueAsBool("bIsBusy", true);
				BB->SetValueAsBool("bInAttackRange", false);
			}
			return EBTNodeResult::Succeeded;
		}
		break;

	case ETalMaskType::Imae:
		if (ASC->TryActivateAbilityByClass(ImaeRushAndRoarAbility))
		{
			if (BB)
			{
				BB->SetValueAsBool("bIsBusy", true);
				BB->SetValueAsBool("bInAttackRange", false);
			}
			return EBTNodeResult::Succeeded;
		}
		break;

	default:;
		break;
	}
	
	return EBTNodeResult::Failed;
}
