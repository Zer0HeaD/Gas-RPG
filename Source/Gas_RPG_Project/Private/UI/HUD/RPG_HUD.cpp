// Copyright Arthur "Zer0HeaD" Zakirov


#include "UI/HUD/RPG_HUD.h"

#include "UI/Widgets/RPG_UserWidget.h"
#include "UI/WidgetController/RPG_OverlayWidgetController.h"

URPG_OverlayWidgetController* ARPG_HUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (!OverlayWidgetController)
	{
		OverlayWidgetController = NewObject<URPG_OverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);

		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

void ARPG_HUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized! Set in a blueprint!"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized! Set in a blueprint!"));

	UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<URPG_UserWidget>(widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	URPG_OverlayWidgetController* overlayWidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(overlayWidgetController);

	widget->AddToViewport();
}
