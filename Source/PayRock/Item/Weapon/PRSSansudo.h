// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Item/BaseItem.h"
#include "PRSSansudo.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API APRSSansudo : public ABaseItem
{
	GENERATED_BODY()

public:

protected:

public:

	APRSSansudo();

	virtual FString GetName() const override;

	virtual TSubclassOf<UGameplayAbility> GetAbilities() const override;
	
	virtual int32 GetAbilityLevel() const override;
	
	virtual int32 GetItemPrice() const override;
protected:
};
