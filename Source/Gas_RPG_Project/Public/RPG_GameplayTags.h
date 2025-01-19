// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "GameplayTags.h"
#include "GameplayTagContainer.h"

/**
 *	RPG_GameplayTags
 *	Sigleton containing native Gameplay Tags
 */

struct FRPG_GameplayTags
{
public:
	static const FRPG_GameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();


	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;
	FGameplayTag Attributes_Secondary_MaxStamina;

	FGameplayTag Attributes_Secondary_Health;
	FGameplayTag Attributes_Secondary_Mana;
	FGameplayTag Attributes_Secondary_Stamina;

	FGameplayTag Attributes_Secondary_Vigor;
	FGameplayTag Attributes_Secondary_Mind;
	FGameplayTag Attributes_Secondary_Endurance;
	FGameplayTag Attributes_Secondary_Strength;
	FGameplayTag Attributes_Secondary_Dexterity;
	FGameplayTag Attributes_Secondary_Intelligence;
	FGameplayTag Attributes_Secondary_Arkane;
	FGameplayTag Attributes_Secondary_Resilience;


	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_StaminaRegeneration;

protected:

private:
	static FRPG_GameplayTags GameplayTags;
};