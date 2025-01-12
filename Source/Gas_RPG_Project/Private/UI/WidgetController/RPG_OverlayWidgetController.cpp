// Copyright Arthur "Zer0HeaD" Zakirov


#include "UI/WidgetController/RPG_OverlayWidgetController.h"
#include "AbilitySystem/RPG_AttributeSet.h"
#include <AbilitySystem/RPG_AbilitySystemComponent.h>

void URPG_OverlayWidgetController::BroadcastInitialValues()
{
	const URPG_AttributeSet* RPG_AttributeSet = CastChecked<URPG_AttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(RPG_AttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(RPG_AttributeSet->GetMaxHealth());

	OnManaChanged.Broadcast(RPG_AttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(RPG_AttributeSet->GetMaxMana());

	OnStaminaChanged.Broadcast(RPG_AttributeSet->GetStamina());
	OnMaxStaminaChanged.Broadcast(RPG_AttributeSet->GetMaxStamina());
}

void URPG_OverlayWidgetController::BindCallbacksToDependencies()
{
	const URPG_AttributeSet* RPG_AttributeSet = CastChecked<URPG_AttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		RPG_AttributeSet->GetHealthAttribute()).AddUObject(this, &URPG_OverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		RPG_AttributeSet->GetMaxHealthAttribute()).AddUObject(this, &URPG_OverlayWidgetController::MaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		RPG_AttributeSet->GetManaAttribute()).AddUObject(this, &URPG_OverlayWidgetController::ManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		RPG_AttributeSet->GetMaxManaAttribute()).AddUObject(this, &URPG_OverlayWidgetController::MaxManaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		RPG_AttributeSet->GetStaminaAttribute()).AddUObject(this, &URPG_OverlayWidgetController::StaminaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		RPG_AttributeSet->GetMaxStaminaAttribute()).AddUObject(this, &URPG_OverlayWidgetController::MaxStaminaChanged);

	Cast<URPG_AbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const auto& Tag : AssetTags)
			{
				// check if tag is a part of "Message" hierarchy
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					// debug
					const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
					GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);

					// broadcast to UI Widget
					const FUIWidgetRow* WidgetRow = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*WidgetRow);
				}
				else
				{
					// debug
					const FString Msg = FString::Printf(TEXT("GE Tag: %s IS NO UNDER 'MESSAGE' HIERARCHY!"), *Tag.ToString());
					GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, Msg);
				}
			}
		}
	);
}

void URPG_OverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void URPG_OverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void URPG_OverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void URPG_OverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}

void URPG_OverlayWidgetController::StaminaChanged(const FOnAttributeChangeData& Data) const
{
	OnStaminaChanged.Broadcast(Data.NewValue);
}

void URPG_OverlayWidgetController::MaxStaminaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxStaminaChanged.Broadcast(Data.NewValue);
}
