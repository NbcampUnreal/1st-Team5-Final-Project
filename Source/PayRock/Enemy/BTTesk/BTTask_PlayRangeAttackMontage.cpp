#include "BTTask_PlayRangeAttackMontage.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "PayRock/Enemy/EnemyCharacter.h"

UBTTask_PlayRangeAttackMontage::UBTTask_PlayRangeAttackMontage()
{
	NodeName = "Play Range Attack Montage";
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_PlayRangeAttackMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* Character = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Character || !RangeAttackMontage)
	{
		return EBTNodeResult::Failed;
	}

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return EBTNodeResult::Failed;

	MontageDuration = AnimInstance->Montage_Play(RangeAttackMontage);
	if (MontageDuration <= 0.f)
	{
		return EBTNodeResult::Failed;
	}
	
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Character);
	if (Enemy && Enemy->HasAuthority())
	{
		Enemy->Multicast_PlayDetectMontage(RangeAttackMontage);
	}

	bIsMontagePlaying = true;
	ElapsedTime = 0.f;

	return EBTNodeResult::InProgress;
}


void UBTTask_PlayRangeAttackMontage::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!bIsMontagePlaying)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	ElapsedTime += DeltaSeconds;
	if (ElapsedTime >= MontageDuration)
	{
		bIsMontagePlaying = false;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_PlayRangeAttackMontage::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (BB && BlackboardKey_RangeAttackReady != NAME_None)
	{
		BB->SetValueAsBool(BlackboardKey_RangeAttackReady, false);
	}
}
