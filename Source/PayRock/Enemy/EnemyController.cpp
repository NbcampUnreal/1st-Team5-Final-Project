#include "EnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "EnemyCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PayRock/Character/PRCharacter.h"

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
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
	AIPerceptionComponent->ConfigureSense(*DamageConfig);

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

	if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(InPawn))
	{
		if (DefaultBehaviorTree)
		{
			UBlackboardComponent* RawBBComp = BlackboardComponent.Get();
			// if (UseBlackboard(DefaultBehaviorTree->BlackboardAsset, RawBBComp))
			// {
			// 	BlackboardComponent = RawBBComp;
			// 	BehaviorTreeComponent->StartTree(*DefaultBehaviorTree);
			// 	RunBehaviorTree(DefaultBehaviorTree);
			// 	GetBlackboardComponent()->SetValueAsVector(TEXT("StartPosition"), Enemy->GetActorLocation());
			// }
			if (DefaultBehaviorTree)
			{
				if (UseBlackboard(DefaultBehaviorTree->BlackboardAsset, RawBBComp))
				{
					BlackboardComponent = RawBBComp;
					RunBehaviorTree(DefaultBehaviorTree); 
					GetBlackboardComponent()->SetValueAsVector(TEXT("StartPosition"), Enemy->GetActorLocation());
				}
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

void AEnemyController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor) return;

	if (Actor->IsA(APRCharacter::StaticClass())==false)
	{
		return;
	}
	if (Actor)
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Actor);
			GetBlackboardComponent()->SetValueAsBool(TEXT("bPlayerDetect"), true);

			GetWorld()->GetTimerManager().ClearTimer(ForgetPlayerTimerHandle);
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(ForgetPlayerTimerHandle, this, &AEnemyController::ClearDetectedPlayer, 2.0f, false);
		}
	}
}

void AEnemyController::ClearDetectedPlayer()
{
	GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), nullptr);
	GetBlackboardComponent()->SetValueAsBool(TEXT("bPlayerDetect"), false);
}
