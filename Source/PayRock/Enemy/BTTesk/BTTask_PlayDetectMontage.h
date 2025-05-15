// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "PayRock/Enemy/EnemyCharacter.h"
#include "BTTask_PlayDetectMontage.generated.h"

UCLASS()
class PAYROCK_API UBTTask_PlayDetectMontage : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	

public:
	UBTTask_PlayDetectMontage();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* DetectMontage;

private:
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	FOnMontageEnded MontageEndedDelegate;
	UBehaviorTreeComponent* CachedOwnerComp = nullptr;
	AAIController* CachedController = nullptr;
	AEnemyCharacter* CachedCharacter = nullptr;
};