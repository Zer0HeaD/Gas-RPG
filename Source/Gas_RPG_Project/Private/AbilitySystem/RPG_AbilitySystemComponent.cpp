// Copyright Arthur "Zer0HeaD" Zakirov


#include "AbilitySystem/RPG_AbilitySystemComponent.h"

void URPG_AbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &URPG_AbilitySystemComponent::EffectApplied);
}

void URPG_AbilitySystemComponent::EffectApplied(
	UAbilitySystemComponent* AbilitySystemComponent, 
	const FGameplayEffectSpec& EffectSpec, 
	FActiveGameplayEffectHandle ActiveffectHandle)
{
	GEngine->AddOnScreenDebugMessage(1, 8.f, FColor::Blue, FString("Effect Applied!"));
}
