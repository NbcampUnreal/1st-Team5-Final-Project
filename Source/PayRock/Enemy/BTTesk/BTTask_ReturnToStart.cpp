#include "BTTask_ReturnToStart.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "PayRock/Enemy/EnemyCharacter.h"


UBTTask_ReturnToStart::UBTTask_ReturnToStart()
{
	NodeName = TEXT("Return To Start");
}

EBTNodeResult::Type UBTTask_ReturnToStart::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return EBTNodeResult::Failed;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB || !BB->GetBlackboardAsset()) return EBTNodeResult::Failed;


	FVector StartPos = BB->GetValueAsVector(TEXT("StartPosition"));
	APawn* Controlled = AICon->GetPawn();
	if (!Controlled) return EBTNodeResult::Failed;

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Controlled);
	if (Enemy)
	{
		Enemy->SetBattleState(false);
	}
	
	BB->ClearValue(FName("TargetActor"));
	BB->SetValueAsBool(FName("bPlayerDetect"), false);
	BB->SetValueAsBool(FName("bInAttackRange"), false);
	BB->SetValueAsBool(FName("bIsBusy"), true);
	BB->SetValueAsBool(FName("bDetect"), false);
	
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(StartPos);
	MoveRequest.SetAcceptanceRadius(5.0f);

	FNavPathSharedPtr NavPath;
	FPathFollowingRequestResult Result = AICon->MoveTo(MoveRequest, &NavPath);

	if (Result.Code == EPathFollowingRequestResult::RequestSuccessful)
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
