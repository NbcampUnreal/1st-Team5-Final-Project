#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "EnemyCharacter.h"
#include "PayRock/Character/PRCharacter.h"
#include "TimerManager.h"
#include "EngineUtils.h"

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

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 1500.f;
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	AIPerceptionComponent->ConfigureSense(*HearingConfig);

	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	SetPerceptionComponent(*AIPerceptionComponent);

	bIsAIActive = false;
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;

	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnTargetPerceptionUpdated);
	}
}

void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!HasAuthority()) return;

	float Distance = 0.f;
	APRCharacter* NearestPlayer = FindNearestPlayer(Distance);

	if (NearestPlayer && Distance <= 4000.f)
	{
		if (!bIsAIActive)
		{
			ActivateAI();
		}
	}
	else
	{
		if (bIsAIActive)
		{
			DeactivateAI();
		}
	}

	for (int32 i = SensedActors.Num() - 1; i >= 0; --i)
	{
		APRCharacter* Player = Cast<APRCharacter>(SensedActors[i]);
		if (!Player || Player->GetbIsDead() || Player->GetbIsExtracted() || Player->GetbIsInvisible())
		{
			if (BlackboardComponent && BlackboardComponent->GetValueAsObject(TEXT("TargetActor")) == Player)
			{
				BlackboardComponent->ClearValue(TEXT("TargetActor"));
				BlackboardComponent->SetValueAsBool(TEXT("bPlayerDetect"), false);
			}
			SensedActors.RemoveAt(i);
		}
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

		float Dist = FVector::Dist(Player->GetActorLocation(), MyPawn->GetActorLocation());
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
	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->SetSenseEnabled(UAISense_Sight::StaticClass(), true);
		AIPerceptionComponent->SetSenseEnabled(UAISense_Hearing::StaticClass(), true);
		AIPerceptionComponent->SetSenseEnabled(UAISense_Damage::StaticClass(), true);
	}

	if (DefaultBehaviorTree)
	{
		RunBehaviorTree(DefaultBehaviorTree);
	}

	bIsAIActive = true;
}

void AEnemyController::DeactivateAI()
{
	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->SetSenseEnabled(UAISense_Sight::StaticClass(), false);
		AIPerceptionComponent->SetSenseEnabled(UAISense_Hearing::StaticClass(), false);
		AIPerceptionComponent->SetSenseEnabled(UAISense_Damage::StaticClass(), false);
	}

	if (BrainComponent)
	{
		BrainComponent->StopLogic(TEXT("Out of Range"));
	}
	ClearBlackboardKeys();
	bIsAIActive = false;
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(InPawn);
	if (!Enemy || !DefaultBehaviorTree) return;

	Enemy->InitAbilityActorInfo();
	Enemy->InitializeDefaultAttributes();
	Enemy->AddCharacterAbilities();

	UBlackboardComponent* BBComponent = nullptr;
	if (UseBlackboard(DefaultBehaviorTree->BlackboardAsset, BBComponent))
	{
		BlackboardComponent = BBComponent;
		RunBehaviorTree(DefaultBehaviorTree);

		if (BlackboardComponent)
		{
			BlackboardComponent->SetValueAsVector(FName("StartPosition"), Enemy->GetActorLocation());
		}
	}
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

void AEnemyController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor || !Actor->IsA(APRCharacter::StaticClass())) return;

	if (!Stimulus.WasSuccessfullySensed())
	{
		SensedActors.Remove(Actor);
		return;
	}

	APRCharacter* Player = Cast<APRCharacter>(Actor);
	if (Player && (Player->GetbIsDead() || Player->GetbIsExtracted() || Player->GetbIsInvisible()))
	{
		SensedActors.Remove(Actor);
		if (BlackboardComponent)
		{
			BlackboardComponent->ClearValue(TEXT("TargetActor"));
			BlackboardComponent->SetValueAsBool(TEXT("bPlayerDetect"), false);
		}
		return;
	}

	if (!SensedActors.Contains(Actor))
	{
		SensedActors.Add(Actor);
		if (BlackboardComponent && !BlackboardComponent->GetValueAsObject(TEXT("TargetActor")))
		{
			BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), Actor);
			BlackboardComponent->SetValueAsBool(TEXT("bPlayerDetect"), true);
		}
	}

	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
	{
		if (GetPawn())
		{
			const FVector Direction = (Stimulus.StimulusLocation - GetPawn()->GetActorLocation()).GetSafeNormal();
			const FRotator LookAtRotation(0.f, Direction.Rotation().Yaw, 0.f);
			GetPawn()->SetActorRotation(LookAtRotation);
		}
	}
	else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Damage>())
	{
		if (GetPawn())
		{
			const FVector Direction = (Actor->GetActorLocation() - GetPawn()->GetActorLocation()).GetSafeNormal();
			const FRotator LookAtRotation(0.f, Direction.Rotation().Yaw, 0.f);
			GetPawn()->SetActorRotation(LookAtRotation);
		}
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

const TArray<AActor*>& AEnemyController::GetSensedActors() const
{
	return SensedActors;
}
