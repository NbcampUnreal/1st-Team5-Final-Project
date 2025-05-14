#include "OneEyedMonsterController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Engine/PointLight.h"
#include "Components/LightComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/PointLightComponent.h"
#include "PayRock/Character/BaseCharacter.h"

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

	float LightLevel = 0.f;
	const FVector TargetLocation = Target->GetActorLocation();
	
	{
		TArray<AActor*> PointLights;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APointLight::StaticClass(), PointLights);

		for (AActor* Light : PointLights)
		{
			APointLight* PointLight = Cast<APointLight>(Light);
			if (!PointLight) continue;

			UPointLightComponent* LightComp = Cast<UPointLightComponent>(Cast<APointLight>(Light)->GetLightComponent());
			if (!LightComp || !LightComp->IsVisible()) continue;
			
			float Distance = FVector::Dist(Light->GetActorLocation(), TargetLocation);
			float Falloff = FMath::Clamp(1.f - (Distance / LightComp->AttenuationRadius), 0.f, 1.f);

			LightLevel += LightComp->Intensity * Falloff;
		}
	}
	
	{
		ABaseCharacter* PlayerCharacter = Cast<ABaseCharacter>(Target);
		if (PlayerCharacter)
		{
			TArray<UPointLightComponent*> TorchLights;
			PlayerCharacter->GetComponents<UPointLightComponent>(TorchLights);

			for (UPointLightComponent* Torch : TorchLights)
			{
				if (!Torch || !Torch->IsVisible()) continue;

				float Distance = FVector::Dist(Torch->GetComponentLocation(), TargetLocation);
				float Falloff = FMath::Clamp(1.f - (Distance / Torch->AttenuationRadius), 0.f, 1.f);

				LightLevel += Torch->Intensity * Falloff;
			}
		}
	}
	
	bool bVisibleInLight = LightLevel >= LightDetectionThreshold;
	BB->SetValueAsBool(TEXT("bPlayerDetect"), bVisibleInLight);
	
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan,
		FString::Printf(TEXT("[OneEye] LightLevel = %.1f → Detect: %s"), LightLevel, bVisibleInLight ? TEXT("True") : TEXT("False")));
}
