// Copyright Arthur "Zer0HeaD" Zakirov


#include "RPG_GameplayTags.h"
#include "GameplayTagsManager.h"

FRPG_GameplayTags FRPG_GameplayTags::GameplayTags;

void FRPG_GameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("Max Health of character"));
	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"),
		FString("Max Mana of character"));
	GameplayTags.Attributes_Secondary_MaxStamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxStamina"),
		FString("Max Stamina of character"));


	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"),
		FString("Increase Health"));
	GameplayTags.Attributes_Primary_Mind = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Mind"),
		FString("Increase Mana"));
	GameplayTags.Attributes_Primary_Endurance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Endurance"),
		FString("Increase Stamina and Equip Load Weight"));
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString("Increase Strength attribute and affect melee weapon"));
	GameplayTags.Attributes_Primary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Dexterity"),
		FString("Increase Dexterity attribute and affect melee/ranged weapon"));
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"),
		FString("Increase Intelligence and magic power"));
	GameplayTags.Attributes_Primary_Arkane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Arkane"),
		FString("Increase Arkane and Arkane power"));
	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"),
		FString("Increase armor and armor penetration"));


	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"),
		FString("Reduces Damage taken, improve block chance"));
	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPenetration"),
		FString("improve ArmorPenetration"));
	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"),
		FString("improve BlockChance"));
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"),
		FString("improve CriticalHitChance"));
	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"),
		FString("improve CriticalHitDamage"));
	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitResistance"),
		FString("improve CriticalHitResistance"));
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"),
		FString("improve HealthRegeneration"));
	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"),
		FString("improve ManaRegeneration"));
	GameplayTags.Attributes_Secondary_StaminaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.StaminaRegeneration"),
		FString("improve StaminaRegeneration"));

	// INPUT TAGS
	GameplayTags.InputTag_RegularAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RegularAttack"),
		FString("InputTag for regular attack: LMB or RB"));
	GameplayTags.InputTag_SpecialAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.SpecialAttack"),
		FString("InputTag for Special attack: RMB or RT"));

	GameplayTags.InputTag_ActiveAbility_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.ActiveAbility_1"),
		FString("InputTag for Active Ability: LMB with Q hold or RB with LB hold"));
	GameplayTags.InputTag_ActiveAbility_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.ActiveAbility_2"),
		FString("InputTag for Active Ability: RMB with Q hold or RT with LB hold"));

	GameplayTags.InputTag_BlockParry = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.BlockParry"),
		FString("InputTag for Block/Parry: Q for parry, Q hold for block"));

	GameplayTags.InputTag_UltimateAbility = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.UltimateAbility"),
		FString("InputTag for Ultimate Ability: MMB or LB + RB"));
	GameplayTags.InputTag_SpecialAbility = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.SpecialAbility"),
		FString("InputTag for Special Ability: R or LT"));

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage"));

	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"),
		FString("Tag granted when hit reacting"));

	GameplayTags.Effects_HeavyHitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HeavyHitReact"),
		FString("Tag granted when hit reacting on heavy weapon/on stun"));
}
