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
	
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayAbility> DetectAbility;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};