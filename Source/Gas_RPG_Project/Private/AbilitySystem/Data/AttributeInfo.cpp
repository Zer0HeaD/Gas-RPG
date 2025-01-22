// Copyright Arthur "Zer0HeaD" Zakirov


#include "AbilitySystem/Data/AttributeInfo.h"

FRPG_AttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FRPG_AttributeInfo& info : AttributeInformation)
	{
		if (info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find info for AttributeTag [%s] on AttributeInfo [%s]"),
			*AttributeTag.ToString(),
			*GetNameSafe(this));
	}
	return FRPG_AttributeInfo();
}
