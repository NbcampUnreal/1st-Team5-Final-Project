// PayRockGames


// PayRockGames

#include "BTS_MasterDetectService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "PayRock/Character/PRCharacter.h"
#include "PayRock/Enemy/EnemyController.h"

UBTS_MasterDetectService::UBTS_MasterDetectService()
{
	NodeName = TEXT("Master Detect Service");
	bNotifyTick = true;
}

void UBTS_MasterDetectService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB || !BB->GetBlackboardAsset()) return;

	APawn* AIPawn = AICon->GetPawn();
	if (!AIPawn) return;
	
	if (BB->GetValueAsVector("StartPosition").IsNearlyZero())
	{
		BB->SetValueAsVector("StartPosition", AIPawn->GetActorLocation());
	}
	
	AActor* TargetActor = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
	APRCharacter* FocusedPlayer = Cast<APRCharacter>(TargetActor);
	if (FocusedPlayer)
	{
		if (FocusedPlayer->GetbIsDead() || FocusedPlayer->GetbIsInvisible() || FocusedPlayer->GetbIsExtracted())
		{
			BB->ClearValue("TargetActor");
			
			bool bAnyValid = false;
			for (TObjectIterator<APRCharacter> It; It; ++It)
			{
				if (!IsValid(*It)) continue;
				APRCharacter* Other = *It;
				if (!Other->GetbIsDead() && !Other->GetbIsInvisible() && !Other->GetbIsExtracted())
				{
					bAnyValid = true;
					break;
				}
			}
			BB->SetValueAsBool("bPlayerDetect", bAnyValid);
		}
	}
	
	FVector StartPos = BB->GetValueAsVector("StartPosition");
	const float Dist = FVector::Dist(StartPos, AIPawn->GetActorLocation());
	if (Dist > ResetDistance)
	{
		BB->ClearValue("TargetActor");
		BB->SetValueAsBool("bPlayerDetect", false);
		BB->SetValueAsBool("bInAttackRange", false);
	}
	
	AEnemyController* EnemyCon = Cast<AEnemyController>(AICon);
	if (EnemyCon)
	{
		const TArray<AActor*>& Candidates = EnemyCon->GetSensedActors();
		float MinDistSq = FLT_MAX;
		AActor* Closest = nullptr;

		for (AActor* Candidate : Candidates)
		{
			if (!IsValid(Candidate)) continue;
			const float Dsq = FVector::DistSquared(Candidate->GetActorLocation(), AIPawn->GetActorLocation());
			if (Dsq < MinDistSq)
			{
				MinDistSq = Dsq;
				Closest = Candidate;
			}
		}

		if (Closest)
		{
			BB->SetValueAsObject("TargetActor", Closest);
			BB->SetValueAsBool("bPlayerDetect", true);
		}
	}
	
	AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (IsValid(PlayerActor))
	{
		const float NearDist = FVector::Dist(PlayerActor->GetActorLocation(), AIPawn->GetActorLocation());
		APRCharacter* PlayerChar = Cast<APRCharacter>(PlayerActor);
		if (PlayerChar && !PlayerChar->GetbIsDead() && !PlayerChar->GetbIsInvisible() && !PlayerChar->GetbIsExtracted())
		{
			const bool bNear = NearDist <= DetectionRadius;
			BB->SetValueAsBool("bPlayerNearWell", bNear);
			if (bNear)
			{
				BB->SetValueAsObject("TargetActor", PlayerChar);
				BB->SetValueAsBool("bPlayerDetect", true);
			}
		}
	}
	
	if (BB->GetValueAsBool("bIsAttacking"))
	{
		float& Elapsed = AttackingElapsedMap.FindOrAdd(AIPawn);
		Elapsed += DeltaSeconds;
		if (Elapsed > MaxAttackingDuration)
		{
			BB->SetValueAsBool("bIsAttacking", false);
			Elapsed = 0.f;
		}
	}
	else
	{
		AttackingElapsedMap.FindOrAdd(AIPawn) = 0.f;
	}

	if (BB->GetValueAsBool("bIsBusy"))
	{
		float& BusyElapsed = BusyElapsedMap.FindOrAdd(AIPawn);
		BusyElapsed += DeltaSeconds;

		if (BusyElapsed > BusyResetTime)
		{
			BB->SetValueAsBool("bIsBusy", false);
			BusyElapsedMap.Remove(AIPawn);
		}
	}
	else
	{
		BusyElapsedMap.Remove(AIPawn);
	}
}

