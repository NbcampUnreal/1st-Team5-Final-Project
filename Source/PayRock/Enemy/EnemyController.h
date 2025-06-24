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
	const TArray<AActor*>& GetSensedActors() const;
	void SetPerceptionActive(bool bEnable);

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> DefaultBehaviorTree;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBlackboardComponent> BlackboardComponent;
	
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
	
	UFUNCTION()
	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	APRCharacter* FindNearestPlayer(float& OutDistance);
	
	void ActivateAI();
	void DeactivateAI();
	
	void CheckPlayerDistance();
	
	void ClearBlackboardKeys();
	
	void ClearDetectedPlayer();
	
	UPROPERTY()
	TArray<AActor*> SensedActors;
	
	UPROPERTY()
	bool bIsAIActive = false;

	FTimerHandle DistanceCheckHandle;
	FTimerHandle ForgetPlayerTimerHandle;
};
