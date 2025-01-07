// Copyright Arthur "Zer0HeaD" Zakirov


#include "Player/RPG_PlayerState.h"
#include <AbilitySystem/RPG_AbilitySystemComponent.h>
#include <AbilitySystem/RPG_AttributeSet.h>

ARPG_PlayerState::ARPG_PlayerState()
{

	AbilitySystemComponent = CreateDefaultSubobject<URPG_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<URPG_AttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* ARPG_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
