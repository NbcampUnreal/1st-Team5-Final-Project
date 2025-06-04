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
}


void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnTargetPerceptionUpdated);
	}
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
		TEXT("TargetActor"),
		TEXT("bPlayerDetect"),
		TEXT("bIsBusy"),
		TEXT("bInAttackRange"),
		TEXT("bDetect"),
		TEXT("bIsAttacking")
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
	
	if (APRCharacter* Player = Cast<APRCharacter>(Actor))
	{
		if (Player->GetbIsDead() || Player->GetbIsExtracted() || Player->GetbIsInvisible())
		{
			SensedActors.Remove(Actor);

			if (BlackboardComponent)
			{
				BlackboardComponent->ClearValue(TEXT("TargetActor"));
				BlackboardComponent->SetValueAsBool(TEXT("bPlayerDetect"), false);
			}
			return;
		}
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