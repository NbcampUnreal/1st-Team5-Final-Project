#include "BTTask_PlayDetectMontage.h"
#include "AIController.h"
#include "Animation/AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PayRock/Enemy/EnemyCharacter.h"

UBTTask_PlayDetectMontage::UBTTask_PlayDetectMontage()
{
	NodeName = "Play Detect Montage";
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_PlayDetectMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	CachedOwnerComp = &OwnerComp;
	CachedController = OwnerComp.GetAIOwner();
	if (!CachedController) return EBTNodeResult::Failed;

	CachedCharacter = Cast<AEnemyCharacter>(CachedController->GetPawn());
	if (!CachedCharacter) return EBTNodeResult::Failed;

	DetectMontage = CachedCharacter->GetRandomDetectMontage();
	if (!DetectMontage) return EBTNodeResult::Failed;

	UAnimInstance* AnimInstance = CachedCharacter->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return EBTNodeResult::Failed;
	
	const float PlayResult = AnimInstance->Montage_Play(DetectMontage);
	if (PlayResult <= 0.f || !AnimInstance->Montage_IsPlaying(DetectMontage))
	{
		return EBTNodeResult::Failed;
	}

	FOnMontageEnded MontageDelegate;
	MontageDelegate.BindUObject(this, &UBTTask_PlayDetectMontage::OnMontageEnded);
	AnimInstance->Montage_SetEndDelegate(MontageDelegate, DetectMontage);
	
	if (UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent())
	{
		BB->SetValueAsBool(FName("bDetect"), true);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_PlayDetectMontage::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!CachedOwnerComp || !CachedCharacter || Montage != DetectMontage) return;
	
	FinishLatentTask(*CachedOwnerComp, bInterrupted ? EBTNodeResult::Failed : EBTNodeResult::Succeeded);
}

void UBTTask_PlayDetectMontage::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	if (CachedCharacter && DetectMontage)
	{
		UAnimInstance* AnimInstance = CachedCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && AnimInstance->Montage_IsPlaying(DetectMontage))
		{
			AnimInstance->Montage_Stop(0.2f, DetectMontage);

			FOnMontageEnded EmptyDelegate;
			AnimInstance->Montage_SetEndDelegate(EmptyDelegate, DetectMontage);
			
		}
	}

	CachedOwnerComp = nullptr;
	CachedController = nullptr;
	CachedCharacter = nullptr;
	DetectMontage = nullptr;
}
