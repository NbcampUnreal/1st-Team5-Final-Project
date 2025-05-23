#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
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
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
	AIPerceptionComponent->ConfigureSense(*DamageConfig);


	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 1500.f;
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerceptionComponent->ConfigureSense(*HearingConfig);
	
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

		if (Enemy && BlackboardComponent)
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

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(GetPawn());
	if (!Enemy) return;
	if (Enemy)
	{
		Enemy->SetBattleState(true);
	}
	const FVector AIPos = Enemy->GetActorLocation();

	const FAISenseID HearingID = UAISense::GetSenseID<UAISense_Hearing>();
	const FAISenseID SightID = UAISense::GetSenseID<UAISense_Sight>();
	const FAISenseID DamageID = UAISense::GetSenseID<UAISense_Damage>();

	if (Stimulus.Type == HearingID)
	{
		if (Stimulus.Strength >= LoudnessThreshold)
		{
			BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), Actor);
			BlackboardComponent->SetValueAsBool(TEXT("bPlayerDetect"), true);
		}
		else if (Stimulus.Strength >= MinLoudnessToReact)
		{
			FRotator LookRot = (Stimulus.StimulusLocation - AIPos).Rotation();
			Enemy->SetActorRotation(FRotator(0.f, LookRot.Yaw, 0.f));
		}
	}
	else if (Stimulus.Type == SightID || Stimulus.Type == DamageID)
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), Actor);
			BlackboardComponent->SetValueAsBool(TEXT("bPlayerDetect"), true);
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
