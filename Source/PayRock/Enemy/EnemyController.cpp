#include "EnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "EnemyCharacter.h"

AEnemyController::AEnemyController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = SightRadius;
	SightConfig->LoseSightRadius = LoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngle;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
	AIPerceptionComponent->ConfigureSense(*DamageConfig);
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyController::OnPerceptionUpdated);
	}
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(InPawn))
	{
		if (DefaultBehaviorTree)
		{
			UBlackboardComponent* RawBBComp = BlackboardComponent.Get();
			if (UseBlackboard(DefaultBehaviorTree->BlackboardAsset, RawBBComp))
			{
				BlackboardComponent = RawBBComp;
				BehaviorTreeComponent->StartTree(*DefaultBehaviorTree);
				GetBlackboardComponent()->SetValueAsVector(TEXT("StartPosition"), Enemy->GetActorLocation());
			}
		}
	}
}

void AEnemyController::OnUnPossess()
{
	Super::OnUnPossess();

	if (BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree();
	}
}

void AEnemyController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* Actor : UpdatedActors)
	{
		if (Actor)
		{
			GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Actor);
		}
	}
}