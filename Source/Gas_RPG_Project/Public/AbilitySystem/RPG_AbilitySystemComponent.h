// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RPG_AbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class GAS_RPG_PROJECT_API URPG_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void AbilityActorInfoSet();


protected:

	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
		const FGameplayEffectSpec& EffectSpec, 
		FActiveGameplayEffectHandle ActiveffectHandle);
};
