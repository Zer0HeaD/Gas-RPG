// Copyright Arthur "Zer0HeaD" Zakirov


#include "UI/Widgets/RPG_UserWidget.h"

void URPG_UserWidget::SetWidgetController(UObject* inWidgetController)
{
	WidgetController = inWidgetController;
	WidgetControllerSet();

}


