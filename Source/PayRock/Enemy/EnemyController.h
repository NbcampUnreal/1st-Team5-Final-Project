#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "EnemyController.generated.h"

class UBehaviorTree;
class UBlackboardComponent;
class APRCharacter;

UCLASS()
class PAYROCK_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyController();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	const TArray<AActor*>& GetSensedActors() const {return SensedActors;};

protected:
	// === AI Core ===
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> DefaultBehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBlackboardComponent> BlackboardComponent;

	UPROPERTY()
	bool bIsAIActive = false;

	// === Perception ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	TObjectPtr<UAISenseConfig_Damage> DamageConfig;

	UPROPERTY(EditAnywhere, Category = "AI|Sight")
	float SightRadius = 1500.f;

	UPROPERTY(EditAnywhere, Category = "AI|Sight")
	float LoseSightRadius = 1800.f;

	UPROPERTY(EditAnywhere, Category = "AI|Sight")
	float PeripheralVisionAngle = 180.f;

	UPROPERTY(EditAnywhere, Category = "AI|Hearing")
	float MinLoudnessToReact = 0.3f;

	UPROPERTY(EditAnywhere, Category = "AI|Hearing")
	float LoudnessThreshold = 0.8f;

	// === Timers ===
	FTimerHandle DistanceCheckHandle;
	FTimerHandle ForgetPlayerTimerHandle;

	// === Runtime State ===
	UPROPERTY()
	TArray<AActor*> SensedActors;

	// === AI Control Logic ===
	UFUNCTION()
	void ActivateAI();

	UFUNCTION()
	void DeactivateAI();

	UFUNCTION()
	void CheckPlayerDistance();

	UFUNCTION()
	void SpawnRespawnPointAt(const FVector& Location);

	// === Blackboard & Perception ===
	UFUNCTION()
	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	APRCharacter* FindNearestPlayer(float& OutDistance);

	UFUNCTION()
	void SetPerceptionActive(bool bEnable);

	UFUNCTION()
	void ClearBlackboardKeys();

	UFUNCTION()
	void ClearDetectedPlayer();
	
};