// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_MasterDetectService.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UBTS_MasterDetectService : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_MasterDetectService();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, Category = "Detect|Reset")
	float MaxAttackingDuration = 2.0f;
	
	UPROPERTY(EditAnywhere, Category = "Detect|Reset")
	float ResetDistance = 4000.0f;

	UPROPERTY(EditAnywhere, Category = "Detect|Well")
	float DetectionRadius = 1300.0f;

	UPROPERTY(EditAnywhere, Category = "BusyReset")
	float BusyResetTime = 2.5f;

private:
	TMap<TObjectPtr<AActor>, float> AttackingElapsedMap;
	TMap<TObjectPtr<AActor>, float> BusyElapsedMap;
};