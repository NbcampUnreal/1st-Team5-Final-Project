// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "EnemyController.generated.h"

class UBehaviorTree;
class UBlackboardComponent;
class UBehaviorTreeComponent;

UCLASS()
class PAYROCK_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyController();
	
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	

	UFUNCTION()
	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> DefaultBehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBlackboardComponent> BlackboardComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;
	UPROPERTY()
	UAISenseConfig_Damage* DamageConfig;

private:
	
	UPROPERTY(EditAnywhere, Category = "AI|Perception")
	float SightRadius = 1500.f;
	UPROPERTY(EditAnywhere, Category = "AI|Perception")
	float LoseSightRadius = 1800.f;
	UPROPERTY(EditAnywhere, Category = "AI|Perception")
	float PeripheralVisionAngle = 90.f;

};
