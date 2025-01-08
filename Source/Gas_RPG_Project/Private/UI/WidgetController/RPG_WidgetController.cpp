// Copyright Arthur "Zer0HeaD" Zakirov


#include "UI/WidgetController/RPG_WidgetController.h"

void URPG_WidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;

	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void URPG_WidgetController::BroadcastInitialValues()
{
}
