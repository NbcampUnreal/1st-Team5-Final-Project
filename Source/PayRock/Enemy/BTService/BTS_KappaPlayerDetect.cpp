// PayRockGames


#include "BTS_KappaPlayerDetect.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PayRock/Character/PRCharacter.h"
#include "Kismet/GameplayStatics.h"

UBTS_KappaPlayerDetect::UBTS_KappaPlayerDetect()
{
	NodeName = TEXT("Kappa Detect Player");
	bNotifyTick = true;
}

void UBTS_KappaPlayerDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APRCharacter* Player = Cast<APRCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	APawn* Controlled = OwnerComp.GetAIOwner()->GetPawn();
	if (!Player || !Controlled) return;

	const float Distance = FVector::Dist(Player->GetActorLocation(), Controlled->GetActorLocation());

	if (Distance <= DetectionRadius)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Player);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bPlayerDetect"), true);
	}
}
