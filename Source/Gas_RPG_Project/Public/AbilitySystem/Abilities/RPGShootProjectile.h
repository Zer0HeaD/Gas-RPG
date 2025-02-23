// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/RPG_DamageGameplayAbility.h"
#include "Actor/RPG_Projectile.h"
#include "RPGShootProjectile.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS()
class GAS_RPG_PROJECT_API URPGShootProjectile : public URPG_DamageGameplayAbility
{
	GENERATED_BODY()
protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(TSubclassOf<ARPG_Projectile> ProjectileClass, FVector MuzzleSocketLocation, FRotator LaunchDirection);

};
