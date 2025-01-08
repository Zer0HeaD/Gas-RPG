// Copyright Arthur "Zer0HeaD" Zakirov


#include "UI/WidgetController/RPG_OverlayWidgetController.h"
#include "AbilitySystem/RPG_AttributeSet.h"

void URPG_OverlayWidgetController::BroadcastInitialValues()
{
	const URPG_AttributeSet* RPG_AttributeSet = CastChecked<URPG_AttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(RPG_AttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(RPG_AttributeSet->GetMaxHealth());

	OnManaChanged.Broadcast(RPG_AttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(RPG_AttributeSet->GetMaxMana());
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
