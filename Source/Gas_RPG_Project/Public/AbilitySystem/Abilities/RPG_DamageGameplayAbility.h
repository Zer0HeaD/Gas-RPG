// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/RPG_GameplayAbility.h"
#include "RPG_DamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class GAS_RPG_PROJECT_API URPG_DamageGameplayAbility : public URPG_GameplayAbility
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage") TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
