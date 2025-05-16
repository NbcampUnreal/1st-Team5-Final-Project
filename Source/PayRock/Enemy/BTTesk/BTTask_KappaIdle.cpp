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
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_KappaIdle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedOwnerComp = &OwnerComp;
	ElapsedTime = 0.f;
	bShouldReturn = false;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;

	StartPosition = BB->GetValueAsVector(TEXT("StartPosition"));
	return EBTNodeResult::InProgress;
}

void UBTTask_KappaIdle::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	APRCharacter* Player = Cast<APRCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	APawn* Controlled = OwnerComp.GetAIOwner()->GetPawn();

	if (!Player || !Controlled) return;

	const FVector PlayerLoc = Player->GetActorLocation();
	const FVector MyLoc = Controlled->GetActorLocation();
	
	const float DistanceToPlayer = FVector::Dist(PlayerLoc, MyLoc);
	if (DistanceToPlayer > MaxDistanceFromPlayer)
	{
		bShouldReturn = true;
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	
	ElapsedTime += DeltaSeconds;
	if (ElapsedTime >= WaitDuration)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

uint16 UBTTask_KappaIdle::GetInstanceMemorySize() const
{
	return sizeof(UBTTask_KappaIdle);
}
