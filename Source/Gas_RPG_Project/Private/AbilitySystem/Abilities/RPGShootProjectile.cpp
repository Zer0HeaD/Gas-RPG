// Copyright Arthur "Zer0HeaD" Zakirov


#include "AbilitySystem/Abilities/RPGShootProjectile.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Interaction/CombatInterface.h>
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "RPG_GameplayTags.h"

void URPGShootProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URPGShootProjectile::SpawnProjectile(TSubclassOf<ARPG_Projectile> ProjectileClass, FVector MuzzleSocketLocation, FRotator LaunchDirection)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(MuzzleSocketLocation);
	SpawnTransform.SetRotation(LaunchDirection.Quaternion());

	ARPG_Projectile* Projectile = GetWorld()->SpawnActorDeferred<ARPG_Projectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

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

	Projectile->DamageEffectSpecHandle = SpecHandle;

	Projectile->FinishSpawning(SpawnTransform);
}
