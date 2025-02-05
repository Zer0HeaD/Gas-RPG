// Copyright Arthur "Zer0HeaD" Zakirov


#include "AbilitySystem/RPG_AbilitySystemGlobals.h"
#include "RPG_AbilityTypes.h"

FGameplayEffectContext* URPG_AbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return  new FRPGGameplayEffectContext(); //UAbilitySystemGlobals::AllocGameplayEffectContext();
}
