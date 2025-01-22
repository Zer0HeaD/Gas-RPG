// Copyright Arthur "Zer0HeaD" Zakirov


#include "AbilitySystem/RPG_AbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/RPG_HUD.h"
#include "Player/RPG_PlayerState.h"
#include "UI/WidgetController/RPG_WidgetController.h"

URPG_OverlayWidgetController* URPG_AbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ARPG_HUD* HUD = Cast<ARPG_HUD>(PC->GetHUD()))
		{
			ARPG_PlayerState* PS = PC->GetPlayerState<ARPG_PlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams FWidgetControllerParams(PC, PS, ASC, AS);

			return HUD->GetOverlayWidgetController(FWidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* URPG_AbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ARPG_HUD* HUD = Cast<ARPG_HUD>(PC->GetHUD()))
		{
			ARPG_PlayerState* PS = PC->GetPlayerState<ARPG_PlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams FWidgetControllerParams(PC, PS, ASC, AS);

			return HUD->GetAttributeMenuWidgetController(FWidgetControllerParams);
		}
	}
	return nullptr;
}
