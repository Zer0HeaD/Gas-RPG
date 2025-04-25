// Copyright Arthur "Zer0HeaD" Zakirov


#include "AbilitySystem/Abilities/RPGExecution.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Interaction/CombatInterface.h>
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "RPG_GameplayTags.h"

void URPGExecution::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

const FGameplayEffectSpecHandle URPGExecution::CalculateDamage()
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return FGameplayEffectSpecHandle();

	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		const UAbilitySystemComponent* Source_ASC =
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());

		check(DamageEffectClass);
		const FGameplayEffectSpecHandle SpecHandle =
			Source_ASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), Source_ASC->MakeEffectContext());

		FRPG_GameplayTags GameplayTags = FRPG_GameplayTags::Get();

		for (auto& Pair : DamageTypes)
		{
			const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
		}
		return SpecHandle;
	}
	else
	{
		return FGameplayEffectSpecHandle();
	}
}
