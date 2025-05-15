#pragma once

#include "CoreMinimal.h"
#include "PayRock/Item/BaseItem.h"
#include "HealPotion.generated.h"

UCLASS()
class PAYROCK_API AHealPotion : public ABaseItem
{
	GENERATED_BODY()
	
public:
	AHealPotion();

	virtual FString GetName() const override;
	virtual TSubclassOf<UGameplayAbility> GetAbilities() const override;
	virtual float GetAbilityLevel() const override;
	virtual int32 GetItemPrice() const override;
};
