#include "BTTask_KappaIdle.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PayRock/Character/PRCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTTask_KappaIdle::UBTTask_KappaIdle()
{
	NodeName = TEXT("Kappa Idle");
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_KappaIdle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedOwnerComp = &OwnerComp;
	bShouldReturn = false;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;

	StartPosition = BB->GetValueAsVector(TEXT("StartPosition"));
	
	OwnerComp.GetWorld()->GetTimerManager().SetTimer(WaitTimerHandle, [this]() {
		if (CachedOwnerComp)
		{
			FinishLatentTask(*CachedOwnerComp, bShouldReturn ? EBTNodeResult::Failed : EBTNodeResult::Succeeded);
		}
	}, WaitDuration, false);

	return EBTNodeResult::InProgress;
}

void UBTTask_KappaIdle::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	APRCharacter* Player = Cast<APRCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	APawn* Controlled = OwnerComp.GetAIOwner()->GetPawn();

	if (!Player || !Controlled) return;

	float Distance = FVector::Dist(Player->GetActorLocation(), Controlled->GetActorLocation());
	
	if (Distance > MaxDistanceFromPlayer)
	{
		bShouldReturn = true;
		OwnerComp.GetWorld()->GetTimerManager().ClearTimer(WaitTimerHandle);
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
