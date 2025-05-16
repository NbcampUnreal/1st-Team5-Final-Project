// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Item/BaseItem.h"
#include "PRGonbang.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API APRGonbang : public ABaseItem
{
	GENERATED_BODY()
public:

	APRGonbang();
	
	virtual FString GetName() const override;

	virtual TSubclassOf<UGameplayAbility> GetAbilities() const override;
	
	virtual int32 GetAbilityLevel() const override;
	
	virtual int32 GetItemPrice() const override;
};
