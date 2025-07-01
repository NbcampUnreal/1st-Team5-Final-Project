// Optimized AEnemyController.cpp

#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "EnemyCharacter.h"
#include "PayRock/Character/PRCharacter.h"
#include "TimerManager.h"
#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AEnemyController::AEnemyController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = SightRadius;
	SightConfig->LoseSightRadius = LoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngle;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	AIPerceptionComponent->ConfigureSense(*SightConfig);

	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
	AIPerceptionComponent->ConfigureSense(*DamageConfig);

	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	SetPerceptionComponent(*AIPerceptionComponent);

	bIsAIActive = false;
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = false;

	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnTargetPerceptionUpdated);
		SetPerceptionActive(false);
	}

	GetWorldTimerManager().SetTimer(DistanceCheckHandle, this, &AEnemyController::CheckPlayerDistance, 1.5f, true);
}

void AEnemyController::CheckPlayerDistance()
{
	if (!HasAuthority()) return;

	float Distance = 0.f;
	APRCharacter* NearestPlayer = FindNearestPlayer(Distance);
	AEnemyCharacter* EnemyChar = Cast<AEnemyCharacter>(GetPawn());

	if (!EnemyChar) return;

	if (Distance <= 2200.f && !bIsAIActive)
	{
		SetPerceptionActive(true);
		ActivateAI();
		EnemyChar->RestoreAnimInstance();
		EnemyChar->bIsSleeping = false;
		
		if (NearestPlayer && BlackboardComponent)
		{
			BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), NearestPlayer);
			BlackboardComponent->SetValueAsBool(TEXT("bPlayerDetect"), true);
		}
	}
	else if (Distance > 3000.f)
	{
		DeactivateAI();
		SetPerceptionActive(false);
		EnemyChar->DisableAnimInstance();

		float CurrentTime = GetWorld()->GetTimeSeconds();
		if (!EnemyChar->bIsSleeping)
		{
			EnemyChar->bIsSleeping = true;
			EnemyChar->SleepStartTime = CurrentTime;
		}
		else if (CurrentTime - EnemyChar->SleepStartTime > EnemyChar->MaxSleepDuration)
		{
			SpawnRespawnPointAt(EnemyChar->GetActorLocation());
			EnemyChar->Destroy();
		}
	}
}


void AEnemyController::SpawnRespawnPointAt(const FVector& Location)
{
	AEnemyCharacter* EnemyChar = Cast<AEnemyCharacter>(GetPawn());
	if (!EnemyChar || !EnemyChar->GetSpawnedActor()) return;

	FVector Offset = FMath::VRand().GetSafeNormal() * 150.f;
	FVector SpawnLoc = Location + Offset;
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	FNavLocation ProjectedLoc;

	if (NavSys && NavSys->ProjectPointToNavigation(SpawnLoc, ProjectedLoc, FVector(300.f)))
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		GetWorld()->SpawnActor<AActor>(EnemyChar->GetSpawnedActor(), ProjectedLoc.Location, FRotator::ZeroRotator, Params);
	}
}

APRCharacter* AEnemyController::FindNearestPlayer(float& OutDistance)
{
	OutDistance = FLT_MAX;
	APRCharacter* NearestPlayer = nullptr;
	AActor* MyPawn = GetPawn();
	if (!MyPawn) return nullptr;

	for (TActorIterator<APRCharacter> It(GetWorld()); It; ++It)
	{
		APRCharacter* Player = *It;
		if (!Player || Player->GetbIsDead() || Player->GetbIsExtracted() || Player->GetbIsInvisible())
			continue;
		
		const float Dist = FVector::Dist(Player->GetActorLocation(), MyPawn->GetActorLocation());
		if (Dist < OutDistance)
		{
			OutDistance = Dist;
			NearestPlayer = Player;
		}
	}

	return NearestPlayer;
}


void AEnemyController::ActivateAI()
{
	if (bIsAIActive || !DefaultBehaviorTree) return;

	if (!BrainComponent || !BrainComponent->IsRunning())
	{
		if (!RunBehaviorTree(DefaultBehaviorTree)) return;
	}

	bIsAIActive = true;
}

void AEnemyController::DeactivateAI()
{
	if (BrainComponent && BrainComponent->IsRunning())
	{
		BrainComponent->StopLogic(TEXT("Deactivated by distance"));
	}
	ClearBlackboardKeys();
	bIsAIActive = false;
}

void AEnemyController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor || !Actor->IsA(APRCharacter::StaticClass())) return;

	APRCharacter* Player = Cast<APRCharacter>(Actor);
	if (!Player || Player->GetbIsDead() || Player->GetbIsExtracted() || Player->GetbIsInvisible())
	{
		ClearDetectedPlayer();
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		if (!bIsAIActive)
		{
			ActivateAI();
		}
		if (BlackboardComponent && !BlackboardComponent->GetValueAsObject(TEXT("TargetActor")))
		{
			BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), Actor);
			BlackboardComponent->SetValueAsBool(TEXT("bPlayerDetect"), true);
		}
	}
	else
	{
		ClearDetectedPlayer();
	}
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!InPawn || !DefaultBehaviorTree)
	{
		return;
	}

	if (!DefaultBehaviorTree->BlackboardAsset)
	{
		return;
	}

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(InPawn);
	if (!Enemy) return;

	Enemy->InitAbilityActorInfo();
	Enemy->InitializeDefaultAttributes();
	Enemy->AddCharacterAbilities();

	UBlackboardComponent* BBComponent = nullptr;
	if (!UseBlackboard(DefaultBehaviorTree->BlackboardAsset, BBComponent) || !BBComponent)
	{
		return;
	}

	BlackboardComponent = BBComponent;
	BlackboardComponent->SetValueAsVector(FName("StartPosition"), Enemy->GetActorLocation());
}

void AEnemyController::OnUnPossess()
{
	Super::OnUnPossess();
	ClearBlackboardKeys();
}

void AEnemyController::ClearBlackboardKeys()
{
	if (!BlackboardComponent) return;
	static const TArray<FName> KeysToClear = {
		TEXT("TargetActor"), TEXT("bPlayerDetect"), TEXT("bIsBusy"),
		TEXT("bInAttackRange"), TEXT("bDetect"), TEXT("bIsAttacking")
	};
	for (const FName& Key : KeysToClear)
	{
		BlackboardComponent->ClearValue(Key);
	}
}

void AEnemyController::ClearDetectedPlayer()
{
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), nullptr);
		BlackboardComponent->SetValueAsBool(TEXT("bPlayerDetect"), false);
	}
}

void AEnemyController::SetPerceptionActive(bool bEnable)
{
	if (!AIPerceptionComponent) return;
	AIPerceptionComponent->SetComponentTickEnabled(bEnable);
	AIPerceptionComponent->SetActive(bEnable);
	AIPerceptionComponent->bAutoActivate = bEnable;
	AIPerceptionComponent->Activate(bEnable);
}