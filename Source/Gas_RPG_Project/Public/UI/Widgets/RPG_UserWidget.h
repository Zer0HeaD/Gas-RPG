// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPG_UserWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAS_RPG_PROJECT_API URPG_UserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable) void SetWidgetController(UObject* inWidgetController);

	UPROPERTY(BlueprintReadOnly) TObjectPtr<UObject> WidgetController;

protected:

	UFUNCTION(BlueprintImplementableEvent) void WidgetControllerSet();
};
