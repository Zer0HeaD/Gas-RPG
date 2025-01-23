// Copyright Arthur "Zer0HeaD" Zakirov


#include "AbilitySystem/RPG_AbilitySystemComponent.h"
#include "RPG_GameplayTags.h"

void URPG_AbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &URPG_AbilitySystemComponent::EffectApplied);
	
	//const FRPG_GameplayTags RPG_GameplayTags = FRPG_GameplayTags::Get();
	//RPG_GameplayTags.Attributes_Secondary_Armor.ToString();

	/*GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, FString::Printf(
		TEXT("Tag: %s"), *RPG_GameplayTags.Attributes_Secondary_Armor.ToString()));*/
}

void URPG_AbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		//GiveAbility(AbilitySpec);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void URPG_AbilitySystemComponent::EffectApplied(
	UAbilitySystemComponent* AbilitySystemComponent, 
	const FGameplayEffectSpec& EffectSpec, 
	FActiveGameplayEffectHandle ActiveffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}
