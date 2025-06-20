#include "OneEyedMonsterController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Engine/PointLight.h"
#include "Components/LightComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "PayRock/Character/BaseCharacter.h"
#include "PayRock/Character/PRCharacter.h"
#include "PayRock/Enemy/EnemyCharacter.h"

AOneEyedMonsterController::AOneEyedMonsterController()
{
	DamageConfig = nullptr;
	PrimaryActorTick.bCanEverTick = false; 
}

void AOneEyedMonsterController::BeginPlay()
{
	Super::BeginPlay();
}

void AOneEyedMonsterController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor || !Actor->IsA(APRCharacter::StaticClass())) return;

	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB) return;

	if (Stimulus.WasSuccessfullySensed())
	{
		AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(GetPawn());
		if (Enemy)
		{
			Enemy->SetBattleState(true);
		}

		BB->SetValueAsObject(TEXT("TargetActor"), Actor);
		BB->SetValueAsBool(TEXT("bPlayerDetectedBySight"), true);
	}
	else
	{
		BB->ClearValue(TEXT("TargetActor"));
		BB->SetValueAsBool(TEXT("bPlayerDetectedBySight"), false);
		BB->SetValueAsBool(TEXT("bPlayerDetectedByLight"), false);
	}
}
