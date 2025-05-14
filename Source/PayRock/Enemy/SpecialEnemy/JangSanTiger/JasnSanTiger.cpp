// PayRockGames


#include "JasnSanTiger.h"

#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "PayRock/Enemy/EnemyController.h"

AJasnSanTiger::AJasnSanTiger()
{
	PrimaryActorTick.bCanEverTick = true;

	TelegraphDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("TelegraphDecal"));
	TelegraphDecal->SetupAttachment(RootComponent);
	TelegraphDecal->SetRelativeLocation(FVector(300.f, 0.f, 10.f));
	TelegraphDecal->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	TelegraphDecal->DecalSize = FVector(1000.f, 200.f, 200.f);
	TelegraphDecal->SetVisibility(false);

	ChargeCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ChargeCollision"));
	ChargeCollision->SetupAttachment(RootComponent);
	ChargeCollision->SetBoxExtent(FVector(50.f, 100.f, 100.f));
	ChargeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ChargeCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	ChargeCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	
	AIControllerClass = AEnemyController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	
}

void AJasnSanTiger::BeginTelegraph()
{
	if (!TelegraphMat && TelegraphDecal)
	{
		TelegraphMat = TelegraphDecal->CreateDynamicMaterialInstance();
	}

	if (TelegraphMat)
	{
		TelegraphMat->SetScalarParameterValue("Length", 0.1f);
		TelegraphMat->SetScalarParameterValue("GradientPower", 2.5f);
		TelegraphMat->SetVectorParameterValue("BaseColor", FLinearColor::Red);
	}

	TelegraphDecal->SetVisibility(true);
	CurrentLength = 0.1f;
	bTelegraphing = true;
}

void AJasnSanTiger::UpdateTelegraph(float DeltaTime)
{
	if (!bTelegraphing || !TelegraphMat) return;

	CurrentLength += DeltaTime * 1.2f;
	CurrentLength = FMath::Clamp(CurrentLength, 0.1f, maxLength);
	TelegraphMat->SetScalarParameterValue("Length", CurrentLength);
}

void AJasnSanTiger::EndTelegraph()
{
	bTelegraphing = false;
	if (TelegraphDecal)
	{
		TelegraphMat->SetScalarParameterValue("Length", 0.1f);
		TelegraphMat->SetScalarParameterValue("GradientPower", 2.5f);
		TelegraphMat->SetVectorParameterValue("BaseColor", FLinearColor::Red);
		TelegraphDecal->SetVisibility(false);
		
	}
}

void AJasnSanTiger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateTelegraph(DeltaTime);
}