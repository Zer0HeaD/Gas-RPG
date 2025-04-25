// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/RPG_DamageGameplayAbility.h"
#include "RPGExecution.generated.h"

/**
 * 
 */
UCLASS()
class GAS_RPG_PROJECT_API URPGExecution : public URPG_DamageGameplayAbility
{
	GENERATED_BODY()
	
protected:

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	const FGameplayEffectSpecHandle CalculateDamage();
};
