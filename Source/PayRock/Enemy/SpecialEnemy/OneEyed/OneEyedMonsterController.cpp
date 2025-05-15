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
	PrimaryActorTick.bCanEverTick = true;
	DamageConfig = nullptr;
}

void AOneEyedMonsterController::BeginPlay()
{
	Super::BeginPlay();
}

void AOneEyedMonsterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckLightBasedDetection();
}

void AOneEyedMonsterController::CheckLightBasedDetection()
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB) return;

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TEXT("TargetActor")));
	if (!Target) return;

	const FVector TargetLocation = Target->GetActorLocation();
	float LightLevel = 0.f;
	
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);

	for (AActor* Actor : AllActors)
	{
		if (!Actor) continue;

		TArray<ULightComponent*> LightComponents;
		Actor->GetComponents<ULightComponent>(LightComponents);

		for (ULightComponent* Light : LightComponents)
		{
			if (!Light || !Light->IsVisible()) continue;

			if (auto* Spot = Cast<USpotLightComponent>(Light))
			{
				FVector LightDir = Spot->GetForwardVector();
				FVector ToTarget = (TargetLocation - Spot->GetComponentLocation()).GetSafeNormal();
				float Angle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(LightDir, ToTarget)));

				if (Angle > Spot->OuterConeAngle) continue;

				float Distance = FVector::Dist(Spot->GetComponentLocation(), TargetLocation);
				float Falloff = FMath::Clamp(1.f - (Distance / Spot->AttenuationRadius), 0.f, 1.f);
				LightLevel += Spot->Intensity * Falloff;
			}
			else if (auto* Point = Cast<UPointLightComponent>(Light))
			{
				float Distance = FVector::Dist(Point->GetComponentLocation(), TargetLocation);
				float Falloff = FMath::Clamp(1.f - (Distance / Point->AttenuationRadius), 0.f, 1.f);
				LightLevel += Point->Intensity * Falloff;
			}
			else if (auto* Dir = Cast<UDirectionalLightComponent>(Light))
			{
				LightLevel += Dir->Intensity; 
			}
		}
	}

	bool bVisibleInLight = LightLevel >= LightDetectionThreshold;
	BB->SetValueAsBool(TEXT("bPlayerDetectedByLight"), bVisibleInLight);

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan,
		FString::Printf(TEXT("[OneEye] LightLevel = %.1f → Detect(Light): %s"), LightLevel, bVisibleInLight ? TEXT("True") : TEXT("False")));
}



void AOneEyedMonsterController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor) return;

	if (!Actor->IsA(APRCharacter::StaticClass())) return;

	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB) return;
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(GetPawn());
	
	if (Enemy)
	{
		Enemy->SetBattleState(true);
	}
	
	BB->SetValueAsObject(TEXT("TargetActor"), Actor);
	BB->SetValueAsBool(TEXT("bPlayerDetectedBySight"), Stimulus.WasSuccessfullySensed());
}