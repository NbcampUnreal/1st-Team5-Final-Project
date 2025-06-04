// PayRockGames


#include "BTTask_PhasePattern.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"

UBTTask_PhasePattern::UBTTask_PhasePattern()
{
	NodeName = TEXT("Phase Pattern");
	bNotifyTaskFinished = true;
	PhaseKey.SelectedKeyName = "CurrentPhase_Int";
}

EBTNodeResult::Type UBTTask_PhasePattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Boss) return EBTNodeResult::Failed;

	UAbilitySystemComponent* ASC = Boss->GetAbilitySystemComponent();
	if (!ASC || !Pase_FirstAbility || !Pase_SecondAbility) return EBTNodeResult::Failed;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;

	const int32 CurrentPhase = BB->GetValueAsInt(PhaseKey.SelectedKeyName);

	bool bActivated = false;

	switch (CurrentPhase)
	{
	case 1:
		bActivated = ASC->TryActivateAbilityByClass(Pase_FirstAbility);
		break;

	case 2:
		bActivated = ASC->TryActivateAbilityByClass(Pase_SecondAbility);
		break;

	default:
			return EBTNodeResult::Failed;
	}

	if (bActivated)
	{
		BB->SetValueAsBool(FName("bIsAttacking"), true);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
