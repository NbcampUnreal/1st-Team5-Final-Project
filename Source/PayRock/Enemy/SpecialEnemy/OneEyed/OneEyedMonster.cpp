// PayRockGames

#include "OneEyedMonster.h"
#include "OneEyedMonsterController.h"
#include "BehaviorTree/BlackboardComponent.h"


AOneEyedMonster::AOneEyedMonster()
{
	AIControllerClass = AOneEyedMonsterController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	TorchLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("TorchLight"));
	
	TorchLight->SetupAttachment(GetMesh(), TEXT("EyeSocket"));
	
	TorchLight->SetIntensity(10000.f);
	TorchLight->SetAttenuationRadius(800.f);
	TorchLight->SetInnerConeAngle(20.f);
	TorchLight->SetOuterConeAngle(60.f);
	TorchLight->SetVisibility(true);

	TorchBeamVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TorchBeamVisual"));
	TorchBeamVisual->SetupAttachment(TorchLight);
	TorchBeamVisual->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	TorchBeamVisual->SetVisibility(true);

	WeaponCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetupAttachment(Weapon, CollisionSocketName);
	WeaponCollision->InitSphereRadius(50.f);
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollision->SetCollisionObjectType(ECC_WorldDynamic);
	WeaponCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	WeaponCollision->SetGenerateOverlapEvents(true);
}


void AOneEyedMonster::BeginPlay()
{
	Super::BeginPlay();
	ScheduleNextBlink();
}

void AOneEyedMonster::ScheduleNextBlink()
{
	GetWorldTimerManager().ClearTimer(BlinkTimerHandle);

	AAIController* AICon = Cast<AAIController>(GetController());
	if (AICon)
	{
		UBlackboardComponent* BB = AICon->GetBlackboardComponent();
		if (BB && BB->GetValueAsBool(FName("bPlayerDetect")))
		{
			TorchLight->SetVisibility(true);
			TorchBeamVisual->SetVisibility(true);

			TorchLight->SetLightColor(AlertLightColor);
	
			UMaterialInstanceDynamic* DynMat = TorchBeamVisual->CreateAndSetMaterialInstanceDynamic(0);
			if (DynMat)
			{
				DynMat->SetVectorParameterValue("Color", AlertLightColor);
			}
	
			return;
		}
		else
		{
			TorchLight->SetLightColor(NormalLightColor);

			UMaterialInstanceDynamic* DynMat = TorchBeamVisual->CreateAndSetMaterialInstanceDynamic(0);
			if (DynMat)
			{
				DynMat->SetVectorParameterValue("Color", NormalLightColor);
			}
		}
		
	}

	
	
	float Delay = FMath::FRandRange(BlinkMin, BlinkMax);
	GetWorldTimerManager().SetTimer(BlinkTimerHandle, this, &AOneEyedMonster::ToggleTorchLight, Delay, false);
} 

void AOneEyedMonster::ToggleTorchLight()
{
	if (TorchLight)
	{
		bool bVisible = TorchLight->IsVisible();
		TorchLight->SetVisibility(!bVisible);
		TorchBeamVisual->SetVisibility(!bVisible);
	}
	ScheduleNextBlink();
}