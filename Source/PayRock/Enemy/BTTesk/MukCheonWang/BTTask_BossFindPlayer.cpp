// PayRockGames


#include "BTTask_BossFindPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"

UBTTask_BossFindPlayer::UBTTask_BossFindPlayer()
{
	NodeName = "Boss Find Player (Montage)";
	bNotifyTick = false;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_BossFindPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return EBTNodeResult::Failed;

	AMukCheonWangCharacter* Boss = Cast<AMukCheonWangCharacter>(AICon->GetPawn());
	if (!Boss) return EBTNodeResult::Failed;

	if (!Boss->GetRoarMontage())
	{
		return EBTNodeResult::Succeeded;
	}

	UAnimInstance* AnimInstance = Boss->GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(Boss->GetRoarMontage(), MontagePlayRate);
		
		FTimerHandle TempHandle;
		Boss->GetWorldTimerManager().SetTimer(TempHandle, FTimerDelegate::CreateLambda([this, &OwnerComp]() {
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}), WaitTime, false);

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}
