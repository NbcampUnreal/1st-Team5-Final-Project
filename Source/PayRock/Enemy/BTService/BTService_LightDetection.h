// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_LightDetection.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTService_LightDetection : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_LightDetection();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Light Detection")
	float DetectionRadius = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Light Detection")
	float LightDetectionThreshold = 1000.f;
};