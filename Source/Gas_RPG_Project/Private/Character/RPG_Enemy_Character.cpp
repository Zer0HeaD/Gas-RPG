// Copyright Arthur "Zer0HeaD" Zakirov


#include "Character/RPG_Enemy_Character.h"
#include <AbilitySystem/RPG_AbilitySystemComponent.h>
#include <AbilitySystem/RPG_AttributeSet.h>
#include "Components/WidgetComponent.h"
#include "AbilitySystem/RPG_AbilitySystemLibrary.h"

ARPG_Enemy_Character::ARPG_Enemy_Character()
{
	AbilitySystemComponent = CreateDefaultSubobject<URPG_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<URPG_AttributeSet>("AttributeSet");

	InfoWidget = CreateDefaultSubobject<UWidgetComponent>("Stat Widget");
	InfoWidget->SetupAttachment(GetRootComponent());
}

void ARPG_Enemy_Character::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();

	if (URPG_UserWidget* RPG_UserWidget = Cast<URPG_UserWidget>(InfoWidget->GetUserWidgetObject()))
	{
		RPG_UserWidget->SetWidgetController(this);
	}


	BindDelegates();
}

void ARPG_Enemy_Character::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<URPG_AbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAttributes();
}

void ARPG_Enemy_Character::InitializeDefaultAttributes() const
{
	URPG_AbilitySystemLibrary::InitializeDefultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void ARPG_Enemy_Character::BindDelegates()
{
	if (const URPG_AttributeSet* RPG_AS = Cast<URPG_AttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RPG_AS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RPG_AS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		OnHealthChanged.Broadcast(RPG_AS->GetHealth());
		OnMaxHealthChanged.Broadcast(RPG_AS->GetMaxHealth());
	}
}

int32 ARPG_Enemy_Character::GetPlayerLevel()
{
	return Level;
}
