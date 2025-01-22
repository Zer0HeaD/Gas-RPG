// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RPG_HUD.generated.h"

class URPG_UserWidget;
class URPG_OverlayWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;
class UAttributeMenuWidgetController;

struct FWidgetControllerParams;
/**
 * 
 */
UCLASS()
class GAS_RPG_PROJECT_API ARPG_HUD : public AHUD
{
	GENERATED_BODY()
	
public:

	URPG_OverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

protected:


private:

	UPROPERTY() TObjectPtr<URPG_UserWidget> OverlayWidget;
	UPROPERTY(EditAnywhere) TSubclassOf<URPG_UserWidget> OverlayWidgetClass;

	UPROPERTY() TObjectPtr<URPG_OverlayWidgetController> OverlayWidgetController;
	UPROPERTY(EditAnywhere) TSubclassOf<URPG_OverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY() TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
	UPROPERTY(EditAnywhere) TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
};
