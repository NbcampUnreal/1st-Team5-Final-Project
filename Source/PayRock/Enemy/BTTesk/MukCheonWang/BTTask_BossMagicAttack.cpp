#include "BTTask_BossMagicAttack.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"

UBTTask_BossMagicAttack::UBTTask_BossMagicAttack()
{
	NodeName = TEXT("Boss Magic Attack");
	bNotifyTaskFinished = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_BossMagicAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedOwnerComp = &OwnerComp;

	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Boss || !MagicAbility) return EBTNodeResult::Failed;

	UAbilitySystemComponent* ASC = Boss->GetAbilitySystemComponent();
	if (!ASC) return EBTNodeResult::Failed;

	CachedASC = ASC;
	ThisAbilityClass = MagicAbility;
	
	AbilityEndedHandle = ASC->OnAbilityEnded.AddLambda([this](const FAbilityEndedData& Data)
	{
		if (ThisAbilityClass && Data.AbilityThatEnded && Data.AbilityThatEnded->GetClass() == ThisAbilityClass)
		{
			if (CachedOwnerComp.IsValid())
			{
				if (UBlackboardComponent* BB = CachedOwnerComp->GetBlackboardComponent())
				{
					BB->SetValueAsBool("bIsAttacking", false);
				}
				FinishLatentTask(*CachedOwnerComp.Get(), EBTNodeResult::Succeeded);
			}

			if (CachedASC.IsValid())
			{
				CachedASC->OnAbilityEnded.Remove(AbilityEndedHandle);
			}
		}
	});

	if (ASC->TryActivateAbilityByClass(MagicAbility))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("bIsAttacking"), true);
		return EBTNodeResult::InProgress;
	}
	
	ASC->OnAbilityEnded.Remove(AbilityEndedHandle);
	return EBTNodeResult::Failed;
}
