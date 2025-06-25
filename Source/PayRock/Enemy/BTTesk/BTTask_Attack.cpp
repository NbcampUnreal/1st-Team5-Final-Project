#include "BTTask_Attack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "PayRock/Enemy/EnemyCharacter.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = "Base Attack";
	bNotifyTaskFinished = false; 
	bCreateNodeInstance = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AICon ? AICon->GetPawn() : nullptr);
	if (!Enemy || !AttackAbility) return EBTNodeResult::Failed;
	if (Enemy->IsDead()) return EBTNodeResult::Failed;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB || !BB->GetBlackboardAsset()) return EBTNodeResult::Failed;

	if (!BB || BB->GetValueAsBool(FName("bIsDead")))
	{
		return EBTNodeResult::Failed;
	}

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(FName("TargetActor")));
	if (Target)
	{
		const FVector Direction = (Target->GetActorLocation() - Enemy->GetActorLocation()).GetSafeNormal();
		FRotator TargetRot = Direction.Rotation();
		TargetRot.Pitch = 0.f;
		TargetRot.Roll = 0.f;

		const FRotator CurrentRot = Enemy->GetActorRotation();
		const float InterpSpeed = 25.f;
		const float DeltaTime = 0.03f;

		const FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, InterpSpeed);
		Enemy->SetActorRotation(NewRot);
	}

	if (AICon)
	{
		AICon->StopMovement();
	}

	UAbilitySystemComponent* ASC = Enemy->GetAbilitySystemComponent();
	if (!ASC) return EBTNodeResult::Failed;

	if (ASC->TryActivateAbilityByClass(AttackAbility))
	{
		if (BB)
		{
			BB->SetValueAsBool(FName("bIsBusy"), true);
			BB->SetValueAsBool(FName("bInAttackRange"), false);
			BB->SetValueAsBool(FName("bIsAttacking"), true);
		}

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
