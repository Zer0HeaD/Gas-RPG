// Copyright Arthur "Zer0HeaD" Zakirov


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/RPG_AttributeSet.h"
#include "RPG_GameplayTags.h"
#include <AbilitySystem/Data/AttributeInfo.h>

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	URPG_AttributeSet* AS = CastChecked<URPG_AttributeSet>(AttributeSet);
	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	URPG_AttributeSet* AS = CastChecked<URPG_AttributeSet>(AttributeSet);
	check(AttributeInfo);
	
	// iterate through array of TagsToAttributes which is filled in URPG_AttributeSet, in constructor
	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(
	const FGameplayTag& AttributeTag, 
	const FGameplayAttribute& Attribute) const
{
	FRPG_AttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
