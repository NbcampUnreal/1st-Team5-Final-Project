// PayRockGames


#include "BTTask_ReturnToStart.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"
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
	if (!BB) return EBTNodeResult::Failed;

	FVector StartPos = BB->GetValueAsVector(TEXT("StartPosition"));
	APawn* Controlled = AICon->GetPawn();
	if (!Controlled) return EBTNodeResult::Failed;
	
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(StartPos);
	MoveRequest.SetAcceptanceRadius(5.0f);

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AICon->GetPawn());
	if (Enemy)
	{
		Enemy->SetBattleState(false);
		BB->SetValueAsBool(FName("bIsBusy"), true);
	}
	
	FNavPathSharedPtr NavPath;
	FPathFollowingRequestResult Result = AICon->MoveTo(MoveRequest, &NavPath);

	if (Result.Code == EPathFollowingRequestResult::RequestSuccessful)
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
