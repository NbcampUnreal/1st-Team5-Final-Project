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
    if (!CachedCharacter || !CachedCharacter->GetRandomDetectMontage()) return EBTNodeResult::Failed;

    DetectMontage = CachedCharacter->GetRandomDetectMontage();
    UAnimInstance* AnimInstance = CachedCharacter->GetMesh()->GetAnimInstance();
    if (!AnimInstance) return EBTNodeResult::Failed;

    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    if (!BB) return EBTNodeResult::Failed;
    
    float PlayResult = AnimInstance->Montage_Play(DetectMontage);
    BB->SetValueAsBool(FName("bDetect"), true);
    if (PlayResult == 0.f)
    {
        return EBTNodeResult::Failed;
    }
    
    MontageEndedDelegate.BindUObject(this, &UBTTask_PlayDetectMontage::OnMontageEnded);
    AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, DetectMontage);

    return EBTNodeResult::InProgress;
}

void UBTTask_PlayDetectMontage::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (!CachedOwnerComp) return;

    EBTNodeResult::Type Result = bInterrupted ? EBTNodeResult::Failed : EBTNodeResult::Succeeded;
    FinishLatentTask(*CachedOwnerComp, Result);
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
        }
    }

    CachedOwnerComp = nullptr;
    CachedController = nullptr;
    CachedCharacter = nullptr;
}
