// Copyright Arthur "Zer0HeaD" Zakirov


#include "Input/RPG_InputConfig.h"

const UInputAction* URPG_InputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FRPGInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find abilityInputAction for InputTag [%s], on InputConfig [%s]"),
			*InputTag.ToString(), *GetNameSafe(this));
	}
	return  nullptr;
}
