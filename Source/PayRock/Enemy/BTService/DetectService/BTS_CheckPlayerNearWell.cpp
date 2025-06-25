// PayRockGames


#include "BTS_CheckPlayerNearWell.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PayRock/Character/PRCharacter.h"
#include "Kismet/GameplayStatics.h"

UBTS_CheckPlayerNearWell::UBTS_CheckPlayerNearWell()
{
	NodeName = "Check Player Near Well";
	bNotifyTick = true;
}

void UBTS_CheckPlayerNearWell::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB || !BB->GetBlackboardAsset()) return;


	AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerActor ) return;

	APawn* Controlled = OwnerComp.GetAIOwner()->GetPawn();
	if (!Controlled) return;

	const float Distance = FVector::Dist(Controlled->GetActorLocation(), PlayerActor ->GetActorLocation());

	APRCharacter* Player = Cast<APRCharacter>(PlayerActor);
	if (!Player || Player->GetbIsDead() || Player->GetbIsExtracted() || Player->GetbIsInvisible())
	{
		return;
	}

	
	bool bInRange = Distance <= DetectionRadius;
	BB->SetValueAsBool(TEXT("bPlayerNearWell"), bInRange);
	
	if (bInRange)
	{
		BB->SetValueAsObject(TEXT("TargetActor"), Player);
		BB->SetValueAsBool(TEXT("bPlayerDetect"), true);
	}
}
