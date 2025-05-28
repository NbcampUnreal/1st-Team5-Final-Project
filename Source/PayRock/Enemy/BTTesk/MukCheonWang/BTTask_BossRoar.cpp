#include "BTTask_BossRoar.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"

UBTTask_BossRoar::UBTTask_BossRoar()
{
	NodeName = "Boss Roar";
	bNotifyTick = true;
	
	ActiveRoarTag = FGameplayTag::RequestGameplayTag("Boss.State.Roaring");
}

EBTNodeResult::Type UBTTask_BossRoar::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	ACharacter* Boss = Cast<ACharacter>(AICon ? AICon->GetPawn() : nullptr);
	if (!Boss || !RoarAbilityClass)
	{
		return EBTNodeResult::Failed;
	}

	if (UAbilitySystemComponent* ASC = Boss->FindComponentByClass<UAbilitySystemComponent>())
	{
		FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(RoarAbilityClass);
		if (!Spec)
		{
			FGameplayAbilitySpec NewSpec(RoarAbilityClass, 1);
			ASC->GiveAbility(NewSpec);
			Spec = ASC->FindAbilitySpecFromClass(RoarAbilityClass);
		}

		if (Spec && ASC->TryActivateAbility(Spec->Handle))
		{
			bIsWaitingForEnd = true;
			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::Failed;
}

void UBTTask_BossRoar::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!bIsWaitingForEnd) return;

	AAIController* AICon = OwnerComp.GetAIOwner();
	ACharacter* Boss = Cast<ACharacter>(AICon ? AICon->GetPawn() : nullptr);
	if (!Boss) return;

	if (UAbilitySystemComponent* ASC = Boss->FindComponentByClass<UAbilitySystemComponent>())
	{
		if (!ASC->HasMatchingGameplayTag(ActiveRoarTag))
		{
			bIsWaitingForEnd = false;
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}
