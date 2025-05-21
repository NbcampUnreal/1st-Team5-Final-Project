// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Item/BaseItem.h"
#include "PRSSangeom.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API APRSSangeom : public ABaseItem
{
	GENERATED_BODY()
	
public:

protected:

public:

	APRSSangeom();

	virtual FString GetName() const override;

	virtual TSubclassOf<UGameplayAbility> GetAbilities() const override;
	
	virtual int32 GetAbilityLevel() const override;
	
	virtual int32 GetItemPrice() const override;
protected:
};
