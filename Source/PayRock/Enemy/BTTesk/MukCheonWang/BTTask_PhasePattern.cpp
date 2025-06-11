#include "BTTask_PhasePattern.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"

UBTTask_PhasePattern::UBTTask_PhasePattern()
{
	NodeName = TEXT("Phase Pattern");
	bNotifyTick = false;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = true;
	PhaseKey.SelectedKeyName = "CurrentPhase_Int";
}

EBTNodeResult::Type UBTTask_PhasePattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Boss) return EBTNodeResult::Failed;

	UAbilitySystemComponent* ASC = Boss->GetAbilitySystemComponent();
	if (!ASC) return EBTNodeResult::Failed;

	const int32 CurrentPhase = OwnerComp.GetBlackboardComponent()->GetValueAsInt(PhaseKey.SelectedKeyName);

	TSubclassOf<UGameplayAbility> AbilityToActivate = nullptr;
	switch (CurrentPhase)
	{
	case 1: AbilityToActivate = Pase_FirstAbility; break;
	case 2: AbilityToActivate = Pase_SecondAbility; break;
	default: return EBTNodeResult::Failed;
	}

	if (!AbilityToActivate) return EBTNodeResult::Failed;

	CachedOwnerComp = &OwnerComp;
	ThisPhaseAbilityClass = AbilityToActivate;
	
	AbilityEndedDelegateHandle = ASC->OnAbilityEnded.AddLambda([this](const FAbilityEndedData& Data)
	{
		if (Data.AbilityThatEnded && Data.AbilityThatEnded->GetClass() == ThisPhaseAbilityClass)
		{
			if (CachedOwnerComp.IsValid() && CachedOwnerComp->GetBlackboardComponent())
			{
				CachedOwnerComp->GetBlackboardComponent()->SetValueAsBool(FName("bIsAttacking"), false);
				FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
			}

			if (UAbilitySystemComponent* InnerASC = Data.AbilityThatEnded->GetCurrentActorInfo()->AbilitySystemComponent.Get())
			{
				InnerASC->OnAbilityEnded.Remove(AbilityEndedDelegateHandle);
			}
		}
	});


	if (ASC->TryActivateAbilityByClass(AbilityToActivate))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("bIsAttacking"), true);
		return EBTNodeResult::InProgress;
	}
	
	ASC->AbilityEndedCallbacks.Remove(AbilityEndedDelegateHandle);
	return EBTNodeResult::Failed;
}
