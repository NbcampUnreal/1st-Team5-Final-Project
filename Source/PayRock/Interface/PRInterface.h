// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PRInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPRInterface : public UInterface
{
	GENERATED_BODY()
};

class PAYROCK_API IPRInterface
{
	GENERATED_BODY()

public:
	virtual void Interact(class APRCharacter* Interactor) = 0;
	virtual FText GetInteractionName() const = 0;
	virtual bool CanInteract(class APRCharacter* Interactor) const { return true; }
};
