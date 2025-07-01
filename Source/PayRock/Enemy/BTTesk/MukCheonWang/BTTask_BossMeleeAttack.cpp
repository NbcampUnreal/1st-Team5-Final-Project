#include "BTTask_BossMeleeAttack.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"

UBTTask_BossMeleeAttack::UBTTask_BossMeleeAttack()
{
	NodeName = TEXT("Boss Melee Attack");
	bNotifyTaskFinished = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_BossMeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedOwnerComp = &OwnerComp;

	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Boss || !MeleeAbility) return EBTNodeResult::Failed;

	UAbilitySystemComponent* ASC = Boss->GetAbilitySystemComponent();
	if (!ASC) return EBTNodeResult::Failed;

	CachedASC = ASC;
	ThisAbilityClass = MeleeAbility;
	
	if (UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent())
	{
		AActor* Target = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
		if (Target)
		{
			FVector Direction = (Target->GetActorLocation() - Boss->GetActorLocation()).GetSafeNormal2D();
			FRotator TargetRot = Direction.Rotation();
			Boss->SetActorRotation(FRotator(0.f, TargetRot.Yaw, 0.f));
		}
	}

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

	if (ASC->TryActivateAbilityByClass(MeleeAbility))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("bIsAttacking"), true);
		return EBTNodeResult::InProgress;
	}

	ASC->OnAbilityEnded.Remove(AbilityEndedHandle);
	return EBTNodeResult::Failed;
}
