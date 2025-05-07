// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "PRCharacter.generated.h"

UCLASS()
class PAYROCK_API APRCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APRCharacter();
	
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

private:
	virtual void InitAbilityActorInfo() override;
	void SetupStimuliSource();

protected:
	//AI감지 관련 함수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionStimuliSourceComponent* StimuliSourceComponent;
};
