// Copyright Arthur "Zer0HeaD" Zakirov


#include "UI/WidgetController/RPG_OverlayWidgetController.h"
#include "AbilitySystem/RPG_AttributeSet.h"

void URPG_OverlayWidgetController::BroadcastInitialValues()
{
	const URPG_AttributeSet* RPG_AttributeSet = CastChecked<URPG_AttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(RPG_AttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(RPG_AttributeSet->GetMaxHealth());
}
