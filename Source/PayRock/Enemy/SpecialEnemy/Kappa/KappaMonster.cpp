
#include "KappaMonster.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


AKappaMonster::AKappaMonster()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CharacterType = ECharacterType::Kappa;
	LeftHandCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHandCollision"));
	check(LeftHandCollisionComp);
	RightHandCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("RightHandCollision"));
	check(RightHandCollisionComp);  
}

void AKappaMonster::BeginPlay()
{
	Super::BeginPlay();

	if (!GetMesh())
	{
		return;
	}

	if (!LeftHandCollisionComp||!RightHandCollisionComp)
	{
		return;
	}
	
	LeftHandCollisionComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftHandSocketName);
	LeftHandCollisionComp->SetRelativeLocation(FVector::ZeroVector);
	LeftHandCollisionComp->SetSphereRadius(40.f);
	LeftHandCollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	LeftHandCollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	LeftHandCollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	LeftHandCollisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	LeftHandCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollisionComp->SetGenerateOverlapEvents(true);

	RightHandCollisionComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightHandSocketName);
	RightHandCollisionComp->SetRelativeLocation(FVector::ZeroVector);
	RightHandCollisionComp->SetSphereRadius(40.f);
	RightHandCollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	RightHandCollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RightHandCollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	RightHandCollisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	RightHandCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCollisionComp->SetGenerateOverlapEvents(true);
}


void AKappaMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKappaMonster::OnPlayerDetected(AActor* DetectedActor)
{
}

