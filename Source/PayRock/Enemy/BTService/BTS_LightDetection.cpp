// PayRockGames


#include "BTS_LightDetection.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/LightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/DirectionalLightComponent.h"

UBTS_LightDetection::UBTS_LightDetection()
{
	NodeName = TEXT("Check Light Level");
	Interval = 0.5f;
	RandomDeviation = 0.5f;
}
void UBTS_LightDetection::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return;

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TEXT("TargetActor")));
	if (!Target) return;

	const FVector TargetLocation = Target->GetActorLocation();
	float LightLevel = 0.f;

	TArray<AActor*> LightActors;
	UGameplayStatics::GetAllActorsOfClass(AICon->GetWorld(), AActor::StaticClass(), LightActors);

	for (AActor* Actor : LightActors)
	{
		if (!Actor || FVector::Dist(Actor->GetActorLocation(), TargetLocation) > DetectionRadius) continue;

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
				float Falloff = Spot->AttenuationRadius > 0.f ? FMath::Clamp(1.f - (Distance / Spot->AttenuationRadius), 0.f, 1.f) : 1.f;
				LightLevel += Spot->Intensity * Falloff;
			}
			else if (auto* Point = Cast<UPointLightComponent>(Light))
			{
				float Distance = FVector::Dist(Point->GetComponentLocation(), TargetLocation);
				float Falloff = Point->AttenuationRadius > 0.f ? FMath::Clamp(1.f - (Distance / Point->AttenuationRadius), 0.f, 1.f) : 1.f;
				LightLevel += Point->Intensity * Falloff;
			}
			else if (auto* Dir = Cast<UDirectionalLightComponent>(Light))
			{
				LightLevel += Dir->Intensity;
			}
		}
	}

	bool bVisible = LightLevel >= LightDetectionThreshold;
	BB->SetValueAsBool(TEXT("bPlayerDetectedByLight"), bVisible);
}
