#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "EnemyController.generated.h"

class UBehaviorTree;
class UBlackboardComponent;
class UBehaviorTreeComponent;
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
	virtual void Tick(float DeltaTime) override;
	
	const TArray<AActor*>& GetSensedActors() const;
	UFUNCTION()
	void ClearBlackboardKeys();

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> DefaultBehaviorTree;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBlackboardComponent> BlackboardComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	TObjectPtr<UAISenseConfig_Damage> DamageConfig;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;
	
	UPROPERTY(EditAnywhere, Category = "AI|Sight")
	float SightRadius = 1500.f;
	
	UPROPERTY(EditAnywhere, Category = "AI|Sight")
	float LoseSightRadius = 1800.f;
	
	UPROPERTY(EditAnywhere, Category = "AI|Sight")
	float PeripheralVisionAngle = 90.f;
	
	UPROPERTY(EditAnywhere, Category = "AI|Hearing")
	float LoudnessThreshold = 0.8f;
	
	UPROPERTY(EditAnywhere, Category = "AI|Hearing")
	float MinLoudnessToReact = 0.3f;
	
	UFUNCTION()
	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	UFUNCTION()
	void ClearDetectedPlayer();
	UFUNCTION()
	void ActivateAI();
	UFUNCTION()
	void DeactivateAI();
	UFUNCTION()
	APRCharacter* FindNearestPlayer(float& OutDistance);

	FTimerHandle ForgetPlayerTimerHandle;

private:
	UPROPERTY()
	TArray<AActor*> SensedActors;
	
	UPROPERTY()
	bool bIsAIActive = false;
};
