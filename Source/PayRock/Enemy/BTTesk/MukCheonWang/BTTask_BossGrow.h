// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_BossGrow.generated.h"


UCLASS()
class PAYROCK_API UBTTask_BossGrow : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_BossGrow();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Boss Grow")
	FVector NewScale = FVector(1.5f, 1.5f, 1.5f);

	UPROPERTY(EditAnywhere, Category = "Boss Grow")
	float NewCapsuleRadius = 80.f;

	UPROPERTY(EditAnywhere, Category = "Boss Grow")
	float NewCapsuleHalfHeight = 160.f;
};