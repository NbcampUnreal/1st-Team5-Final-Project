
#include "KappaMonster.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


AKappaMonster::AKappaMonster()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AKappaMonster::BeginPlay()
{
	Super::BeginPlay();
}

void AKappaMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKappaMonster::OnPlayerDetected(AActor* DetectedActor)
{
}

