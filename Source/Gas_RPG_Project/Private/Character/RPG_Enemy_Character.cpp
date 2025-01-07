// Copyright Arthur "Zer0HeaD" Zakirov


#include "Character/RPG_Enemy_Character.h"
#include <AbilitySystem/RPG_AbilitySystemComponent.h>
#include <AbilitySystem/RPG_AttributeSet.h>

ARPG_Enemy_Character::ARPG_Enemy_Character()
{
	AbilitySystemComponent = CreateDefaultSubobject<URPG_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<URPG_AttributeSet>("AttributeSet");
}

void ARPG_Enemy_Character::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}
