// PayRockGames


#include "PRInputConfig.h"

const UInputAction* UPRInputConfig::FindAbilityInputActionByTag(const FGameplayTag& InputTag, bool bLogIfNotFound) const
{
	for (const FPRInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogIfNotFound)
	{
		UE_LOG(LogTemp, Error,
			TEXT("UPRInputConfig::FindAbilityInputActionByTag : InputTag [%s] on InputConfig [%s]"),
			*InputTag.ToString(), *GetNameSafe(this))
	}

	return nullptr;
}
