// Copyright Arthur "Zer0HeaD" Zakirov


#include "AbilitySystem/RPG_AbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/RPG_HUD.h"
#include "Player/RPG_PlayerState.h"
#include "UI/WidgetController/RPG_WidgetController.h"
#include "Game/RPG_GameModeBase.h"
#include "AbilitySystem/RPG_AbilitySystemComponent.h"

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

void URPG_AbilitySystemLibrary::InitializeDefultAttributes(
	const UObject* WorldContextObject, 
	ECharacterClass CharacterClass, 
	float Level,
	UAbilitySystemComponent* ASC)
{
	if (ARPG_GameModeBase* RPG_GM = Cast<ARPG_GameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		AActor* AvatarActor = ASC->GetAvatarActor();

		UCharacterClassInfo* CharacterClassInfo = RPG_GM->CharacterClassInfo;
		check(CharacterClassInfo);
		FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

		// APPLY PRIMARY ATTRIBUTES IN GM
		FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
		PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
		const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = 
			ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

		// APPLY SECONDARY ATTRIBUTES IN GM
		FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
		SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
		const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle =
			ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

		// APPLY VITAL ATTRIBUTES IN GM
		FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
		VitalAttributesContextHandle.AddSourceObject(AvatarActor);
		const FGameplayEffectSpecHandle VitalAttributesSpecHandle =
			ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
	}
}
