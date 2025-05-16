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

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp;

	UPROPERTY()
	AAIController* CachedController;

	UPROPERTY()
	class AEnemyCharacter* CachedCharacter;

	UPROPERTY()
	UAnimMontage* DetectMontage;

	FDelegateHandle MontageEndedHandle;
};