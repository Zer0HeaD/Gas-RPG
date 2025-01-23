// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RPG_AbilitySystemComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /* AssetTags */)

/**
 * 
 */
UCLASS()
class GAS_RPG_PROJECT_API URPG_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

protected:

	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
		const FGameplayEffectSpec& EffectSpec, 
		FActiveGameplayEffectHandle ActiveffectHandle);
};
