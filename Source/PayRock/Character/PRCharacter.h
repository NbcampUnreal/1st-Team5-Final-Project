// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PRCharacter.generated.h"

UCLASS()
class PAYROCK_API APRCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APRCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

private:
	virtual void InitAbilityActorInfo() override;
};
