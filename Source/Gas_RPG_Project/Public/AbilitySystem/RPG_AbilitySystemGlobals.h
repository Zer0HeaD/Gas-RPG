// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "RPG_AbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class GAS_RPG_PROJECT_API URPG_AbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;

public:

};
