
#include "KappaMonster.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


AKappaMonster::AKappaMonster()
{
	PrimaryActorTick.bCanEverTick = true;

	HeadCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("HeadCollision"));
	check(HeadCollisionComp);  
}

void AKappaMonster::BeginPlay()
{
	Super::BeginPlay();

	if (!GetMesh())
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Mesh is NULL in KappaMonster"));
		return;
	}

	if (!HeadCollisionComp)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ HeadCollisionComp is NULL"));
		return;
	}
	
	if (!HeadSocketName.IsNone())
	{
		HeadCollisionComp->AttachToComponent(
			GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, HeadSocketName);
	}
	
	HeadCollisionComp->SetRelativeLocation(FVector::ZeroVector);
	HeadCollisionComp->SetSphereRadius(40.f);
	HeadCollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	HeadCollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	HeadCollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	HeadCollisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	HeadCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HeadCollisionComp->SetGenerateOverlapEvents(true);
}


void AKappaMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKappaMonster::OnPlayerDetected(AActor* DetectedActor)
{
}

